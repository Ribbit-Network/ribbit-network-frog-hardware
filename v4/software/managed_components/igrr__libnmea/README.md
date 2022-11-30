# NMEA parser component for ESP-IDF

This is a wrapper around [libnmea](https://github.com/jacketizer/libnmea),
in the form of an [ESP-IDF](https://github.com/espressif/esp-idf) component.
It works with any chip supported in ESP-IDF: ESP32, ESP32-S2, ESP32-S3, ESP32-C3.

## Usage

There are two usage options:

1. Use [idf-component-manager](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html). Simply run `idf.py add-dependency igrr/libnmea==required_version` in your project directory.

OR

2. Clone the component into the `components` directory of your project,
or add it as a submodule.
See [libnmea documentation](https://github.com/jacketizer/libnmea#how-to-use-it)
for more details.

This component uses CMake build system. It works with ESP-IDF v3.3 and v4.x.

## Example

Example project is provided inside `example` directory. It works the same way
as `parse_stdin.c` example from libnmea, except that it reads NMEA sentences
from UART.

Connect the TXD pin of GPS receiver to GPIO21 of an ESP32 board.
For other chips (ESP32-S2, ESP32-C3), change the pin number to any free GPIO.

Build and flash the example. Decoded NMEA messages will be displayed in the console.

## License

[libnmea](https://github.com/jacketizer/libnmea), this component, and the
example project are licensed under MIT License.
