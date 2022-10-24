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
#include "driver/gpio.h"
#if CONFIG_GOLIOTH_BLE_SERVICE_ENABLED
#include "ble.h"
#endif
#include "golioth.h"

#include "scd30.h"

#define TAG "golioth_example"

#define i2c_power 7

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

// Current firmware version
static const char* _current_version = "1.2.3";

// Configurable via LightDB State at path "desired/my_config"
int32_t _my_config = 0;

// Configurable via Settings service, key = "LOOP_DELAY_S"
int32_t _loop_delay_s = 10;

static void on_client_event(golioth_client_t client, golioth_client_event_t event, void* arg) {
    ESP_LOGI(
            TAG,
            "Golioth client %s",
            event == GOLIOTH_CLIENT_EVENT_CONNECTED ? "connected" : "disconnected");
}

// Callback function for asynchronous get request of LightDB path "my_int"
static void on_get_my_int(
        golioth_client_t client,
        const golioth_response_t* response,
        const char* path,
        const uint8_t* payload,
        size_t payload_size,
        void* arg) {
    // It's a good idea to check the response status, to make sure the request didn't time out.
    if (response->status != GOLIOTH_OK) {
        ESP_LOGE(TAG, "on_get_my_int status = %s", golioth_status_to_str(response->status));
        return;
    }

    // Now we can use a helper function to convert the binary payload to an integer.
    int32_t value = golioth_payload_as_int(payload, payload_size);
    ESP_LOGI(TAG, "Callback got my_int = %d", value);
}

// Callback function for asynchronous observation of LightDB path "desired/my_config"
static void on_my_config(
        golioth_client_t client,
        const golioth_response_t* response,
        const char* path,
        const uint8_t* payload,
        size_t payload_size,
        void* arg) {
    if (response->status != GOLIOTH_OK) {
        return;
    }

    // Payload might be null if desired/my_config is deleted, so ignore that case
    if (golioth_payload_is_null(payload, payload_size)) {
        return;
    }

    int32_t desired_value = golioth_payload_as_int(payload, payload_size);
    ESP_LOGI(TAG, "Cloud desires %s = %d. Setting now.", path, desired_value);
    _my_config = desired_value;
    golioth_lightdb_delete_async(client, path, NULL, NULL);
}

static golioth_rpc_status_t on_double(
        const char* method,
        const cJSON* params,
        uint8_t* detail,
        size_t detail_size,
        void* callback_arg) {
    if (cJSON_GetArraySize(params) != 1) {
        return RPC_INVALID_ARGUMENT;
    }
    int num_to_double = cJSON_GetArrayItem(params, 0)->valueint;
    snprintf((char*)detail, detail_size, "{ \"value\": %d }", 2 * num_to_double);
    return RPC_OK;
}

static golioth_settings_status_t on_setting(
        const char* key,
        const golioth_settings_value_t* value) {
    ESP_LOGD(TAG, "Received setting: key = %s, type = %d", key, value->type);

    if (0 == strcmp(key, "LOOP_DELAY_S")) {
        // This setting is expected to be an int, return an error if it's not
        if (value->type != GOLIOTH_SETTINGS_VALUE_TYPE_INT) {
            return GOLIOTH_SETTINGS_VALUE_FORMAT_NOT_VALID;
        }

        // This setting must be in range [1, 100], return an error if it's not
        if (value->i32 < 1 || value->i32 > 100) {
            return GOLIOTH_SETTINGS_VALUE_OUTSIDE_RANGE;
        }

        // Setting has passed all checks, so apply it to the loop delay
        ESP_LOGI(TAG, "Setting loop delay to %d s", value->i32);
        _loop_delay_s = value->i32;
        return GOLIOTH_SETTINGS_SUCCESS;
    }

    // If the setting is not recognized, we should return an error
    return GOLIOTH_SETTINGS_KEY_NOT_RECOGNIZED;
}


void scd30_task(void *pvParameters)
{
    i2c_dev_t dev = {0};

    ESP_ERROR_CHECK(scd30_init_desc(&dev, 0, CONFIG_SCD30_I2C_MASTER_SDA, CONFIG_SCD30_I2C_MASTER_SCL));

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

void app_main(void) {

    // The adafruit dev board we are using has an I2C Power Switch on Pin 7.
    /*
    gpio_pad_select_gpio(i2c_power);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);
    gpio_set_level(7, 1); 
    */
   // There is some error in the above code. Modify it to check for and log the error.
    esp_err_t err = gpio_pad_select_gpio(i2c_power);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error selecting GPIO %d: %s", i2c_power, esp_err_to_name(err));
    }
    err = gpio_set_direction(7, GPIO_MODE_OUTPUT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error setting direction for GPIO %d: %s", i2c_power, esp_err_to_name(err));
    }
    err = gpio_set_level(7, 1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error setting level for GPIO %d: %s", i2c_power, esp_err_to_name(err));
    }

    // Initialize NVS first. For this example, it is assumed that WiFi and Golioth
    // PSK credentials are stored in NVS.
    nvs_init();

#if CONFIG_GOLIOTH_BLE_SERVICE_ENABLED
    // Initialize BLE stack, which can be used for provisioning WiFi and Golioth credentials
    ble_init("golioth_esp32");
#endif

    // Create a background shell/CLI task (type "help" to see a list of supported commands)
    shell_start();

    // Set up the SCD30 task
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreatePinnedToCore(scd30_task, "test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

    // If the credentials haven't been set in NVS, we will wait here for the user
    // to input them via the shell.
    if (!nvs_credentials_are_set()) {
        while (1) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            ESP_LOGW(TAG, "WiFi and golioth credentials are not set");
            ESP_LOGW(TAG, "Use the shell settings commands to set them, then restart");
            vTaskDelay(portMAX_DELAY);
        }
    }

    // Initialize WiFi and wait for it to connect
    wifi_init(nvs_read_wifi_ssid(), nvs_read_wifi_password());
    wifi_wait_for_connected();

    // Now we are ready to connect to the Golioth cloud.
    //
    // To start, we need to create a client. The function golioth_client_create will
    // dynamically create a client and return a handle to it.
    //
    // The client itself runs in a separate task, so once this function returns,
    // there will be a new task running in the background.
    //
    // As soon as the task starts, it will try to connect to Golioth using the
    // CoAP protocol over DTLS, with the PSK ID and PSK for authentication.
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

    // Register a callback function that will be called by the client task when
    // connect and disconnect events happen.
    //
    // This is optional, but can be useful for synchronizing operations on connect/disconnect
    // events. For this example, the on_client_event callback will simply log a message.
    golioth_client_register_event_callback(client, on_client_event, NULL);

    // At this point, we have a client that can be used to interact with Golioth services:
    //      Logging
    //      Over-the-Air (OTA) firmware updates
    //      LightDB state
    //      LightDB Stream

    // We'll start by logging a message to Golioth.
    //
    // This is an "asynchronous" function, meaning that this log message will be
    // copied into a queue for later transmission by the client task, and this function
    // will return immediately. Any functions provided by this SDK ending in _async
    // will have the same meaning.
    //
    // The last two arguments are for an optional callback, in case the user wants to
    // be notified of when the log has been received by the Golioth server. In this
    // case we set them to NULL, which makes this a "fire-and-forget" log request.
    golioth_log_info_async(client, "app_main", "Hello, World!", NULL, NULL);

    // We can also log messages "synchronously", meaning the function will block
    // until one of 3 things happen (whichever comes first):
    //
    //  1. We receive a response to the request from the server
    //  2. The user-provided timeout expires
    //  3. The default client task timeout expires (GOLIOTH_COAP_RESPONSE_TIMEOUT_S)
    //
    // In this case, we will block for up to 2 seconds waiting for the server response.
    // We'll check the return code to know whether a timeout happened.
    //
    // Any function provided by this SDK ending in _sync will have the same meaning.
    golioth_status_t status = golioth_log_warn_sync(client, "app_main", "Sync log", 5);
    if (status != GOLIOTH_OK) {
        ESP_LOGE(TAG, "Error in golioth_log_warn_sync: %s", golioth_status_to_str(status));
    }

    // For OTA, we will spawn a background task that will listen for firmware
    // updates from Golioth and automatically update firmware on the device using
    // Espressif's OTA library.
    //
    // This is optional, but most real applications will probably want to use this.
    golioth_fw_update_init(client, _current_version);

    // There are a number of different functions you can call to get and set values in
    // LightDB state, based on the type of value (e.g. int, bool, float, string, JSON).
    golioth_lightdb_set_int_async(client, "my_int", 42, NULL, NULL);
    status = golioth_lightdb_set_string_sync(client, "my_string", "asdf", 4, 5);
    if (status != GOLIOTH_OK) {
        ESP_LOGE(TAG, "Error setting string: %s", golioth_status_to_str(status));
    }

    // Read back the integer we set above
    int32_t readback_int = 0;
    status = golioth_lightdb_get_int_sync(client, "my_int", &readback_int, 5);
    if (status == GOLIOTH_OK) {
        ESP_LOGI(TAG, "Synchronously got my_int = %d", readback_int);
    } else {
        ESP_LOGE(TAG, "Synchronous get my_int failed: %s", golioth_status_to_str(status));
    }

    // To asynchronously get a value from LightDB, a callback function must be provided
    golioth_lightdb_get_async(client, "my_int", on_get_my_int, NULL);

    // We can also "observe" paths in LightDB state. The Golioth cloud will notify
    // our client whenever the resource at that path changes, without needing
    // to poll.
    //
    // This can be used to implement the "digital twin" concept that is common in IoT.
    //
    // In this case, we will observe the path desired/my_config for changes.
    // The callback will read the value, update it locally, then delete the path
    // to indicate that the desired state was processed (the "twins" should be
    // in sync at that point).
    //
    // If you want to try this out, log into Golioth console (console.golioth.io),
    // go to the "LightDB State" tab, and add a new item for desired/my_config.
    // Once set, the on_my_config callback function should be called here.
    golioth_lightdb_observe_async(client, "desired/my_config", on_my_config, NULL);

    // LightDB Stream functions are nearly identical to LightDB state.
    golioth_lightdb_stream_set_int_async(client, "my_stream_int", 15, NULL, NULL);

    // We can register Remote Procedure Call (RPC) methods. RPCs allow
    // remote users to "call a function" on the device.
    //
    // In this case, the device provides a "double" method, which takes an integer input param,
    // doubles it, then returns the resulting value.
    golioth_rpc_register(client, "double", on_double, NULL);

    // We can register a callback for persistent settings. The Settings service
    // allows remote users to manage and push settings to devices that will
    // be stored in device flash.
    //
    // When the cloud has new settings for us, the on_setting function will be called
    // for each setting.
    golioth_settings_register_callback(client, on_setting);

    // Now we'll just sit in a loop and update a LightDB state variable every
    // once in a while.
    ESP_LOGI(TAG, "Entering endless loop");
    int32_t counter = 0;
    char sbuf[21];
    while (1) {
        golioth_lightdb_set_int_async(client, "counter", counter, NULL, NULL);
        snprintf(sbuf, sizeof(sbuf), "Sending hello! %d", counter);
        golioth_log_info_async(client, "app_main", sbuf, NULL, NULL);
        counter++;
        vTaskDelay(_loop_delay_s * 1000 / portTICK_PERIOD_MS);
    };
}
