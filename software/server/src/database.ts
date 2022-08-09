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
      "CREATE TABLE IF NOT EXISTS sensorReadings(name TEXT, value TEXT)"
    );
  }

  async addSensorReading(reading: SensorReading) {
    await this.db.exec(
      'INSERT INTO sensorReadings VALUES (":CO2 :Temperature :Relative_Humidity :Latitude :Longitude :Altitude :scd_temp_offset :baro_temp :baro_pressure_hpa :scd30_pressure_mbar :scd30_alt_m")',
      reading
    );
  }
}

export const db = new AppDAO();
