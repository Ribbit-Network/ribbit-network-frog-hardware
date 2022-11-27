cmake_minimum_required(VERSION 3.5)
list(APPEND EXTRA_COMPONENT_DIRS third_party/golioth-firmware-sdk/port/esp_idf/components)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(golioth-esp-idf-external-app)
