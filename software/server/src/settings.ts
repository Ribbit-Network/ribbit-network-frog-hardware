import { Request, Response } from "express";
import { db } from "./database";

export const getSettings = async (req: Request, res: Response) => {
  res.setHeader("Access-Control-Allow-Origin", "*");

  console.log("[API] /getSettings");

  const uuid = await db.getPlatformUUID();

  const settings = await db.db.all("SELECT * from settings");

  console.log(settings);

  res.json({
    uuid,
    settings,
  });

  console.log("[API:Success] /getSettings");
};
