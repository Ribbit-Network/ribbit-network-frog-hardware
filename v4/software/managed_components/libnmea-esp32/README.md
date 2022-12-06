# NMEA parser component for ESP-IDF

This is a wrapper around [libnmea](https://github.com/jacketizer/libnmea), in the form of an [ESP-IDF](https://github.com/espressif/esp-idf) component. It works with any chip supported in ESP-IDF: ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C2.

## Usage

This component uses CMake build system. It works with ESP-IDF v4.x or later.

See [libnmea documentation](https://github.com/jacketizer/libnmea#how-to-use-it) for more details about libnmea API.

There are two ways to use this component:

1. Use [idf-component-manager](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html). Simply run `idf.py add-dependency igrr/libnmea==required_version` in your project directory.

2. Clone the component into the `components` directory of your project, or add it as a submodule.

## Example

Example project is provided inside `example` directory. It works the same way as `parse_stdin.c` example from libnmea, except that it reads NMEA sentences from UART or I2C.

Configure the example as explained in the sections below, then build and flash it. Decoded NMEA messages will be displayed in the console.

### Using with a UART connected GPS

Connect the TXD pin of GPS receiver to GPIO21 of an ESP32 board. You can change the number to any other unused GPIO. The pin number can be changed in menuconfig (under "libnmea example configuration" menu) or directly in the code.

### Using with an I2C connected GPS

The example also works with an I2C connected PA1010D GPS module (e.g. [this one](https://www.adafruit.com/product/4415)). To use I2C interface instead of UART, select it in menuconfig under "libnmea example configuration" menu. Set the SDA and SCL pin numbers, as well as the I2C address of the module.

## License

[libnmea](https://github.com/jacketizer/libnmea), this component, and the
example project are licensed under MIT License.
