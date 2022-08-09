export interface SensorReading {
  co2: number;
  temp: number;
  relative_humidity: number;
  lat: number;
  lon: number;
  alt: number;
  scd_temp_offset: number;
  baro_temp: number;
  baro_pressure_hpa: number;
  scd30_pressure_mbar: number;
  scd30_alt_m: number;
}
