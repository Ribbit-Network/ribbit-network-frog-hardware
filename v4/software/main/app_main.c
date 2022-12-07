/*
 * Copyright (c) 2022 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <esp_err.h>
#include "nvs.h"
#include "shell.h"
#include "wifi.h"
#include "golioth.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#include "scd30.h"
#include "dps310.h"

#include "nmea.h"
#include "gpgll.h"
#include "gpgga.h"
#include "gprmc.h"
#include "gpgsa.h"
#include "gpvtg.h"
#include "gptxt.h"
#include "gpgsv.h"


#define TAG "frog_sensor"

#define i2c_power 7

#define I2C_FREQ_HZ 100000

#define I2C_PORT 0
#define CONFIG_I2C_MASTER_SDA 3
#define CONFIG_I2C_MASTER_SCL 4

#define DPS_310_I2C_ADDR 0x77

#define PA1010D_ADDR 0x10

#define UART_RX_BUF_SIZE (1024)

static uint8_t s_i2c_link_buf[I2C_LINK_RECOMMENDED_SIZE(2)];
static char s_buf[UART_RX_BUF_SIZE + 1];
static size_t s_total_bytes;

static void on_client_event(golioth_client_t client, golioth_client_event_t event, void* arg) {
    ESP_LOGI(
            TAG,
            "Golioth client %s",
            event == GOLIOTH_CLIENT_EVENT_CONNECTED ? "connected" : "disconnected");
}

void scd30_task(void *pvParameters)
{
    i2c_dev_t dev = {0};

    ESP_ERROR_CHECK(scd30_init_desc(&dev, I2C_PORT, CONFIG_I2C_MASTER_SDA, CONFIG_I2C_MASTER_SCL));

    uint16_t version, major_ver, minor_ver;
    ESP_ERROR_CHECK(scd30_read_firmware_version(&dev, &version));

    major_ver = (version >> 8) & 0xf;
    minor_ver = version & 0xf;

    ESP_LOGI(TAG, "SCD30 Firmware Version: %d.%d", major_ver, minor_ver);

    ESP_LOGI(TAG, "Starting continuous measurement");
    ESP_ERROR_CHECK(scd30_trigger_continuous_measurement(&dev, 0));

    float co2, temperature, humidity;
    bool data_ready;
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));

        scd30_get_data_ready_status(&dev, &data_ready);

        if (data_ready)
        {
            esp_err_t res = scd30_read_measurement(&dev, &co2, &temperature, &humidity);
            if (res != ESP_OK)
            {
                ESP_LOGE(TAG, "Error reading results %d (%s)", res, esp_err_to_name(res));
                continue;
            }

            if (co2 == 0)
            {
                ESP_LOGW(TAG, "Invalid sample detected, skipping");
                continue;
            }

            ESP_LOGI(TAG, "CO2: %.0f ppm", co2);
            ESP_LOGI(TAG, "Temperature: %.2f °C", temperature);
            ESP_LOGI(TAG, "Humidity: %.2f %%", humidity);
        }
    }
}

void nmea_init_interface(void)
{
    const i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = CONFIG_I2C_MASTER_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = CONFIG_I2C_MASTER_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &conf));
    esp_err_t err = i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to install driver: %s", esp_err_to_name(err));
        return;
    }
}

static esp_err_t i2c_read_byte(char* result)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create_static(s_i2c_link_buf, sizeof(s_i2c_link_buf));
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PA1010D_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, (uint8_t*) result, 1);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete_static(cmd);
    return err;
}

void nmea_read_line(char** out_line_buf, size_t *out_line_len, int timeout_ms)
{
	*out_line_buf = NULL;
	*out_line_len = 0;

	bool line_start_found = false;

	while (true) {
		char b;
		esp_err_t err = i2c_read_byte(&b);
		if (err != ESP_OK) {
			ESP_LOGE(TAG, "Failed to read byte over I2C: %s (0x%x)", esp_err_to_name(err), err);
			return;
		}

		if (b == 0) {
			/* No byte available yet, wait a bit and retry */
			vTaskDelay(pdMS_TO_TICKS(10));
			continue;
		}

		if (!line_start_found) {
			if (b != '$') {
				continue;
			} else {
				line_start_found = true;
			}
		}

		s_buf[s_total_bytes] = b;
		++s_total_bytes;

		if (b == '\n') {
			*out_line_buf = s_buf;
			*out_line_len = s_total_bytes;
			s_total_bytes = 0;
			return;
		}

		if (s_total_bytes == sizeof(s_buf)) {
			ESP_LOGE(TAG, "Line buffer overflow");
			s_total_bytes = 0;
		}
	}
}

static void read_and_parse_nmea()
{
    while (1) {
		char fmt_buf[32];
        nmea_s *data;

        char* start;
        size_t length;
        nmea_read_line(&start, &length, 100 /* ms */);
        if (length == 0) {
        	continue;
        }
        /* handle data */
        data = nmea_parse(start, length, 0);
        if (data == NULL) {
            printf("Failed to parse the sentence!\n");
            printf("  Type: %.5s (%d)\n", start+1, nmea_get_type(start));

            nmea_free(data);
        } else {
            // log the data
            if (data->errors != 0) {
                printf("WARN: The sentence struct contains parse errors!\n");
            }

            // if (NMEA_GPGGA == data->type) {
            //     printf("GPGGA sentence\n");
            //     nmea_gpgga_s *gpgga = (nmea_gpgga_s *) data;
            //     printf("Number of satellites: %d\n", gpgga->n_satellites);
            //     printf("Altitude: %f %c\n", gpgga->altitude,
            //             gpgga->altitude_unit);
            //     printf("Longitude:\n");
            //     printf("  Degrees: %d\n", gpgga->longitude.degrees);
            //     printf("  Minutes: %f\n", gpgga->longitude.minutes);
            //     printf("  Cardinal: %c\n", (char) gpgga->longitude.cardinal);
            //     printf("Latitude:\n");
            //     printf("  Degrees: %d\n", gpgga->latitude.degrees);
            //     printf("  Minutes: %f\n", gpgga->latitude.minutes);
            //     printf("  Cardinal: %c\n", (char) gpgga->latitude.cardinal);
            // }

            if (NMEA_GPRMC == data->type) {
                printf("GPRMC sentence\n");
                
                // For latitude, if cardinal is 'S', then degrees is negative
                nmea_gprmc_s *pos = (nmea_gprmc_s *) data;
                // For longitude, take pos -> longitude.degrees and make it negative if pos -> longitude.cardinal is 'W'
                // For latitude, take pos -> latitude.degrees and make it negative if pos -> latitude.cardinal is 'S'
                int longitude = pos -> longitude.degrees;
                int latitude = pos -> latitude.degrees;
                if (pos -> longitude.cardinal == 'W') {
                    longitude = -longitude;
                }
                if (pos -> latitude.cardinal == 'S') {
                    latitude = -latitude;
                }
                printf("  Longitude: %d, Latitude: %d\n", longitude, latitude);
            }

            nmea_free(data);
        }
    }
}

void gps_task(void *pvParameters)
{
    //nmea_init_interface();
    read_and_parse_nmea();
}

void dps310_task(void *pvParameters)
{
    bool sensor_ready = false;
    bool temperature_ready = false;
    bool pressure_ready = false;
    bool coef_ready = false;
    float temperature = 0;
    float pressure = 0;
    esp_err_t err = ESP_FAIL;

    /* Initialize dps310_config_t with DPS310_CONFIG_DEFAULT() macro, which
     * sets default values.
     */
    dps310_config_t config = DPS310_CONFIG_DEFAULT();

    /* Create and initialize the device descriptor with zero */
    dps310_t dev;
    memset(&dev, 0, sizeof(dps310_t));

    /* Modify dps310_config_t */
    config.tmp_oversampling = DPS310_TMP_PRC_128;
    config.pm_oversampling = DPS310_PM_PRC_128;

    /* Initialize the device descriptor */
    ESP_LOGI(TAG, "Initializing the device descriptor");
    err = dps310_init_desc(&dev, DPS_310_I2C_ADDR, I2C_PORT, CONFIG_I2C_MASTER_SDA, CONFIG_I2C_MASTER_SCL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "dps310_init_desc(): %s", esp_err_to_name(err));
        goto fail;
    }

    /* Initialize the device. */
    ESP_LOGI(TAG, "Initializing the device");
    err = dps310_init(&dev, &config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "dps310_init(): %s", esp_err_to_name(err));
        goto fail;
    }

    /* ensure the sensor is ready and coefficients, or COEF, are also ready.
     * */
    ESP_LOGI(TAG, "Waiting for the sensor to be ready for measurement");
    do
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        if (!sensor_ready)
        {
            err = dps310_is_ready_for_sensor(&dev, &sensor_ready);
            if (err != ESP_OK)
            {
                goto fail;
            }
        }

        if (!coef_ready)
        {
            err = dps310_is_ready_for_coef(&dev, &coef_ready);
            if (err != ESP_OK)
            {
                goto fail;
            }
        }
    } while (!sensor_ready || !coef_ready);

    /* read COEF once, which is used to compensate the raw value. The COEF
     * values are kept in the device descriptor.
     */
    err = dps310_get_coef(&dev);
    if (err != ESP_OK)
    {
        goto fail;
    }

    ESP_LOGI(TAG, "Starting the loop");
    while (1) {

        /* Temperature command mode.
         *
         * The sensor measures temperature once, stores the raw value in a
         * resister, and sets TMP_RDY bit. The sensor goes back to standby mode
         * after measurement.
         */
        ESP_LOGI(TAG, "Setting manual temperature measurement mode");
        err = dps310_set_mode(&dev, DPS310_MODE_COMMAND_TEMPERATURE);
        if (err != ESP_OK)
        {
            goto fail;
        }

        /* wait for the result by polling TMP_RDY bit */
        ESP_LOGI(TAG, "Waiting for the temperature value to be ready");
        do
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            err = dps310_is_ready_for_temp(&dev, &temperature_ready);
            if (err != ESP_OK)
            {
                goto fail;
            }
        } while (!temperature_ready);

        /* Read the result of temperature measurement */
        err = dps310_read_temp(&dev, &temperature);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "dps310_read_temp(): %s", esp_err_to_name(err));
            goto fail;
        }
        ESP_LOGI(TAG, "Temperature: %0.2f °C", temperature);

        /* Pressure command mode
         *
         * The sensor measures pressure once, stores the raw value in a resister,
         * and sets PRS_RDY bit. The sensor goes back to standby mode after
         * measurement.
         */
        ESP_LOGI(TAG, "Setting manual pressure measurement mode");
        err = dps310_set_mode(&dev, DPS310_MODE_COMMAND_PRESSURE);
        if (err != ESP_OK)
        {
            goto fail;
        }

        /* wait for the result by polling PRS_RDY bit */
        ESP_LOGI(TAG, "Waiting for the pressure value to be ready");
        do
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            err = dps310_is_ready_for_pressure(&dev, &pressure_ready);
            if (err != ESP_OK)
            {
                goto fail;
            }
        } while (!pressure_ready);

        /* Read the result of pressure measurement, and compensate the result with
         * temperature and COEF.
         *
         * Note that dps310_read_pressure() reads temperature *and* pressure
         * values for compensation, including oversampling rates.
         *
         * This implies:
         *
         * * temperature measurement must be done brefore dps310_read_pressure()
         *   at least once.
         * * the function is slow.
         */
        err = dps310_read_pressure(&dev, &pressure);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "dps310_read_pressure(): %s", esp_err_to_name(err));
            goto fail;
        }
        ESP_LOGI(TAG, "Pressure: %0.2f Pa", pressure);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

fail:
    ESP_LOGE(TAG, "Halting due to error");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void) {
    // The adafruit dev board we are using has an I2C Power Switch on Pin 7.
    gpio_pad_select_gpio(i2c_power);
    esp_err_t err = gpio_set_direction(7, GPIO_MODE_OUTPUT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error setting direction for GPIO %d: %s", i2c_power, esp_err_to_name(err));
    }
    err = gpio_set_level(7, 1);


    nvs_init();
    shell_start();

    if (!nvs_credentials_are_set()) {
        while (1) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            ESP_LOGW(TAG, "WiFi and golioth credentials are not set");
            ESP_LOGW(TAG, "Use the shell settings commands to set them, then restart");
            vTaskDelay(portMAX_DELAY);
        }
    }

    wifi_init(nvs_read_wifi_ssid(), nvs_read_wifi_password());
    wifi_wait_for_connected();

    const char* psk_id = nvs_read_golioth_psk_id();
    const char* psk = nvs_read_golioth_psk();

    golioth_client_config_t config = {
            .credentials = {
                    .auth_type = GOLIOTH_TLS_AUTH_TYPE_PSK,
                    .psk = {
                            .psk_id = psk_id,
                            .psk_id_len = strlen(psk_id),
                            .psk = psk,
                            .psk_len = strlen(psk),
                    }}};
    golioth_client_t client = golioth_client_create(&config);
    assert(client);

    golioth_client_register_event_callback(client, on_client_event, NULL);

    int32_t iteration = 0;

    ESP_ERROR_CHECK(i2cdev_init());

    xTaskCreatePinnedToCore(scd30_task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(dps310_task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(gps_task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

    while (1) {
        golioth_lightdb_set_int_async(client, "iteration", iteration, NULL, NULL);
        iteration++;
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    };
}
