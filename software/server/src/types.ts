export interface SensorReading {
  CO2: number;
  Temperature: number;
  Relative_Humidity: number;
  Latitude: number;
  Longitude: number;
  Altitude: number;
  scd_temp_offset: number;
  baro_temp: number;
  baro_pressure_hpa: number;
  scd30_pressure_mbar: number;
  scd30_alt_m: number;
}
