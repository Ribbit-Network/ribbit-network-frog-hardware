cmake_minimum_required(VERSION 3.12)

# add libnmea component from the top level directory
set(EXTRA_COMPONENT_DIRS "../")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(nmea_example)
