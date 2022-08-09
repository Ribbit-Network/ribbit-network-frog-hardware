import express, { Express, Request, Response } from "express";
import dotenv from "dotenv";

dotenv.config();

const app: Express = express();
const port = process.env.PORT || 3004;

app.use("/", express.static("dashboard/build"));

app.get("/heartbeat", (req: Request, res: Response) => {
  res.setHeader("access-control-allow-origin", "http://localhost:3000");

  res.send("Express + TypeScript Server");
});

app.listen(port, () => {
  console.log(`⚡️[server]: Server is running at https://localhost:${port}`);
});
