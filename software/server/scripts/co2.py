import json
import logging
import os
import time

def main() -> None:

    data = {}
    data["CO2"] = 1
    data["Temperature"] = 32.2
    data["Relative_Humidity"] = 100
    data["Latitude"] = -34.1
    data["Longitude"] = 11.1
    data["Altitude"] = 100
    data["scd_temp_offset"] = -2
    data["baro_temp"] = 32.6
    data["baro_pressure_hpa"] = 360
    data["scd30_pressure_mbar"] = 800
    data["scd30_alt_m"] = 110
    print(data)


if __name__ == "__main__":
    main()
