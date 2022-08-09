import express, { Express, Request, Response } from "express";
import dotenv from "dotenv";
import { getAllSensorReadings, getSensorData } from "./sensorData";

dotenv.config();

const app: Express = express();
const port = process.env.PORT || 80;

app.use("/", express.static("dashboard/build"));

app.get("/heartbeat", (req: Request, res: Response) => {
  res.send("Alive!");
});

app.get("/sensorData", getSensorData);

app.get("/getAllSensorReadings", getAllSensorReadings);

app.listen(port, () => {
  console.log(`⚡️[server]: Server is running at https://localhost:${port}`);
});
