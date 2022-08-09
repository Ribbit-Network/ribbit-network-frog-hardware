import sqlite3 from "sqlite3";
import { open, Database } from "sqlite";
import { SensorReading } from "./types";
import { makeAutoObservable } from "mobx";

class AppDAO {
  db: Database;

  constructor() {
    this.init();

    makeAutoObservable(this);
  }

  async init() {
    console.log("Initializing db");
    sqlite3.verbose();

    this.db = await open({
      driver: sqlite3.Database,
      filename: "./database.sqlite",
    });

    console.log("[DB] Initializing settings table...");
    await this.db.run(
      "CREATE TABLE IF NOT EXISTS settings(name TEXT, value TEXT)"
    );

    console.log("[DB] Initializing sensorReadings table...");
    await this.db.run(
      "CREATE TABLE IF NOT EXISTS sensorReadings(timestamp TEXT, co2 TEXT, temp TEXT, relative_humidity TEXT, lat TEXT, lon TEXT, alt TEXT, scd_temp_offset TEXT, baro_temp TEXT, baro_pressure_hpa TEXT, scd30_pressure_mbar TEXT, scd30_alt_m TEXT)"
    );
  }

  async addSensorReading(reading: SensorReading) {
    const {
      co2,
      temp,
      relative_humidity,
      baro_temp,
      baro_pressure_hpa,
      lat,
      lon,
      scd30_pressure_mbar,
      scd30_alt_m,
      alt,
      scd_temp_offset,
    } = reading;

    await this.db.exec(
      `INSERT INTO sensorReadings VALUES (${Date.now()}, ${co2}, ${temp}, ${relative_humidity}, ${lat}, ${lon}, ${alt}, ${scd_temp_offset}, ${baro_temp}, ${baro_pressure_hpa}, ${scd30_pressure_mbar}, ${scd30_alt_m})`,
      reading
    );
  }
  async getAllSensorReadings(): Promise<SensorReading[]> {
    return this.db.all<SensorReading[]>("SELECT * FROM sensorReadings");
  }
}

export const db = new AppDAO();
