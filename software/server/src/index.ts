import express, { Express, Request, Response } from "express";
import dotenv from "dotenv";
const { spawn } = require("child_process");

dotenv.config();

const app: Express = express();
const port = process.env.PORT || 80;

app.use("/", express.static("dashboard/build"));

app.get("/heartbeat", (req: Request, res: Response) => {
  res.send("Alive!");
});

app.get("/sensorData", (req, res) => {
  console.log("[API] /sensorData");

  const python = spawn("py", ["./scripts/co2.py"]);

  python.stdout.on("data", (data) => {
    res.json(data.toString());

    console.log("[API:Success] /sensorData");
    return;
  });
});

app.listen(port, () => {
  console.log(`⚡️[server]: Server is running at https://localhost:${port}`);
});
