#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "nmea_example.h"
#include "nmea.h"

#define I2C_NUM                CONFIG_EXAMPLE_I2C_NUM
#define I2C_SDA_PIN            CONFIG_EXAMPLE_I2C_SDA_PIN
#define I2C_SCL_PIN            CONFIG_EXAMPLE_I2C_SCL_PIN
#define I2C_FREQ_HZ            100000
#define I2C_RX_BUF_SIZE        (1024)
#define I2C_SLAVE_ADDR         CONFIG_EXAMPLE_I2C_SLAVE_ADDR


static char s_buf[I2C_RX_BUF_SIZE + 1];
static size_t s_total_bytes;

static uint8_t s_i2c_link_buf[I2C_LINK_RECOMMENDED_SIZE(2)];

static const char *TAG = "example_i2c";

void nmea_example_init_interface(void)
{
    const i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_SCL_PIN,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0));
}

static esp_err_t i2c_read_byte(char *result)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create_static(s_i2c_link_buf, sizeof(s_i2c_link_buf));
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_SLAVE_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, (uint8_t *) result, 1);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete_static(cmd);
    return err;
}

void nmea_example_read_line(char **out_line_buf, size_t *out_line_len, int timeout_ms)
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
