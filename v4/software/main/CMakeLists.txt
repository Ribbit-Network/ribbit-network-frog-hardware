set(EXTRA_COMPONENT_DIRS $ENV{HOME}/third_party/esp-idf-lib/components)

idf_component_register(
    INCLUDE_DIRS
        "."
    SRCS
        "app_main.c"
        "shell.c"
        "wifi.c"
        "nvs.c"
    PRIV_REQUIRES
        "golioth_sdk"
        "console"
        "spi_flash"
        "nvs_flash"
        "json"
        "driver"
        "esp_hw_support"
        "scd30"
        "dps310"
)
