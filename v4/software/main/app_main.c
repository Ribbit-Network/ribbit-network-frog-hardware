/*
 * Copyright (c) 2022 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs.h"
#include "shell.h"
#include "wifi.h"
#include "golioth.h"
#include "driver/gpio.h"

#define TAG "golioth_example"

#define i2c_power 7

static void on_client_event(golioth_client_t client, golioth_client_event_t event, void* arg) {
    ESP_LOGI(
            TAG,
            "Golioth client %s",
            event == GOLIOTH_CLIENT_EVENT_CONNECTED ? "connected" : "disconnected");
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
    while (1) {
        golioth_lightdb_set_int_async(client, "iteration", iteration, NULL, NULL);
        iteration++;
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    };
}
