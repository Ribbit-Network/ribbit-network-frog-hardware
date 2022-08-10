import sqlite3 from "sqlite3";
import { open, Database } from "sqlite";
import { SensorReading } from "./types";
import { makeAutoObservable } from "mobx";
import { v5, v4 as uuidv4 } from "uuid";

class AppDAO {
  public db: Database;

  platformUUID?: string;

  constructor() {
    this.init();

    makeAutoObservable(this);
  }

  async init() {
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
      "CREATE TABLE IF NOT EXISTS sensorReadings(uuid TEXT, timestamp TEXT, co2 TEXT, temp TEXT, relative_humidity TEXT, lat TEXT, lon TEXT, alt TEXT, scd_temp_offset TEXT, baro_temp TEXT, baro_pressure_hpa TEXT, scd30_pressure_mbar TEXT, scd30_alt_m TEXT)"
    );

    const platformUUID = await this.getPlatformUUID();
    console.log("[DB] Platform UUID recognized:", platformUUID);
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

    // generating uuid using a platformUUID namespace
    const uuid = v5(uuidv4(), await this.getPlatformUUID());

    await this.db.exec(
      `INSERT INTO sensorReadings VALUES (${uuid}, ${Date.now()}, ${co2}, ${temp}, ${relative_humidity}, ${lat}, ${lon}, ${alt}, ${scd_temp_offset}, ${baro_temp}, ${baro_pressure_hpa}, ${scd30_pressure_mbar}, ${scd30_alt_m})`,
      reading
    );
  }

  async getAllSensorReadings(): Promise<SensorReading[]> {
    return this.db.all<SensorReading[]>("SELECT * FROM sensorReadings");
  }

  async getPlatformUUID(): Promise<string | undefined> {
    if (this.platformUUID) return this.platformUUID;

    const platformUUID = await this.db.get(
      "SELECT name, value FROM settings WHERE name == 'platformUUID'"
    );

    if (platformUUID?.value) {
      this.platformUUID = platformUUID.value;
    } else {
      const newUUID = uuidv4();
      console.log("[DB] initializing platform UUID", newUUID);
      await this.db.exec(
        `INSERT INTO settings VALUES ("platformUUID", "${newUUID}")`
      );

      this.platformUUID = newUUID;
    }

    return this.platformUUID;
  }
}

export const db = new AppDAO();
