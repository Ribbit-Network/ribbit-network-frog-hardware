/*
 * Copyright (c) 2022 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <esp_err.h>
#include "nvs.h"
#include "shell.h"
#include "wifi.h"
#include "golioth.h"
#include "driver/gpio.h"

#include "scd30.h"
#include "dps310.h"

#define TAG "frog_sensor"

#define i2c_power 7

#define I2C_PORT 0
#define CONFIG_I2C_MASTER_SDA 3
#define CONFIG_I2C_MASTER_SCL 4

#define DPS_310_I2C_ADDR 0x77

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
    /*
    gpio_pad_select_gpio(i2c_power);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);
    gpio_set_level(7, 1); 
    */
   // There is some error in the above code. Modify it to check for and log the error.
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

    xTaskCreatePinnedToCore(scd30_task, "test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(dps310_task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

    while (1) {
        golioth_lightdb_set_int_async(client, "iteration", iteration, NULL, NULL);
        iteration++;
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    };
}
