import json
import logging
import os
import time

def main() -> None:

    data = {}
    data["co2"] = 1
    data["temp"] = 32.2
    data["relative_humidity"] = 100
    data["lat"] = -34.1
    data["lon"] = 11.1
    data["alt"] = 100
    data["scd_temp_offset"] = -2
    data["baro_temp"] = 32.6
    data["baro_pressure_hpa"] = 360
    data["scd30_pressure_mbar"] = 800
    data["scd30_alt_m"] = 110
    print(data)


if __name__ == "__main__":
    main()
