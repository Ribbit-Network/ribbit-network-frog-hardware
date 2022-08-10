import { Request, Response } from "express";
import { db } from "./database";

export const getSettings = async (req: Request, res: Response) => {
  res.setHeader("Access-Control-Allow-Origin", "*");

  console.log("[API] /getSettings");

  const uuid = await db.getPlatformUUID();

  const settings = await db.db.all("SELECT * from settings");

  console.log(settings);

  res.json({
    // uuid,
    settings,
  });

  console.log("[API:Success] /getSettings");
};

export const postSettings = async (req: Request, res: Response) => {
  console.log("[API] /postSettings");

  console.log(req.body);

  // TODO
  // const nameValue = [
  //   {
  //     name: "platformName",
  //     value: req.body.platformName,
  //   },
  //   {
  //     name: "hostname",
  //     value: req.body.hostname,
  //   },
  //   {
  //     name: "frequency",
  //     value: req.body.frequency,
  //   },
  // ];

  // const settings = await db.db.exec(
  //   " into settings VALUES (name, value)",
  //   nameValue
  // );
  // const settings = await db.db.exec(
  //   "REPLACE into settings (:name, :value)",
  //   req.body
  // );

  // console.log(settings);

  // res.json({
  //   uuid,
  //   settings,
  // });

  res.send("ok");

  console.log("[API:Success] /postSettings");
};
