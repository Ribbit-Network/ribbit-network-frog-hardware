import { spawn } from "child_process";
import JSON5 from "json5";
import { SensorReading } from "./types";
import { db } from "./database";
import { Request, Response } from "express";

export const getSensorData = async (req: Request, res: Response) => {
  console.log("[API] /sensorData");

  const python = spawn("py", ["./scripts/co2.py"]);

  python.stdout.on("data", async (data: Buffer) => {
    const sensorReading = JSON5.parse(data.toString()) as SensorReading;

    await db.addSensorReading(sensorReading);

    res.json(sensorReading);

    console.log("[API:Success] /sensorData");
  });
};

export const getAllSensorReadings = async (req: Request, res: Response) => {
  console.log("[API] /getAllSensorReadings");

  res.json(await db.getAllSensorReadings());

  console.log("[API:Success] /getAllSensorReadings");
};
