import { Request, Response } from "express";
import { db } from "./database";
import * as fs from "fs";
const exec = require("child_process").exec;

export const postWifiSettings = async (req: Request, res: Response) => {
  res.setHeader("Access-Control-Allow-Origin", "*");

  console.log("[WIFI] /postWifiSettings");

  const values = {
    ssid: "",
    passkey: "",
  };

  const timestamp = Number(new Date());
  const wpa_supplicant_file = "/etc/wpa_supplicant/wpa_supplicant.conf";
  let file = wpa_supplicant_file + ".new-" + timestamp;
  console.log("Creating new file (" + file + ")");
  console.log("Writing settings/values: " + JSON.stringify(values));

  let contents = "";
  contents += "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n";
  contents += "update_config=1\n\n";
  contents += "network={\n";
  contents += '\tssid="' + values.ssid + '"\n';

  if (values.passkey != null && values.passkey.length > 0) {
    contents += '\tpsk="' + values.passkey + '"\n';
  }
  contents += "}\n";

  fs.writeFile(file, contents, function (err) {
    if (err) {
      console.log("Error writing contents to new wpa supplicant file");
      console.log(err);
      res.status(500).send(err);
      return;
    }

    restartInterface();

    console.log("The file was saved!");
    console.log("[WIFI:Success] /postWifiSettings");
    res.status(200).send("done");
  });
};

const restartInterface = () => {
  console.log("Calling ifdown wlan0");
  exec("ifdown wlan0", (error, stdout, stderr) => {
    console.log("(ifdown) ERROR: " + error);
    console.log("(ifdown) STDERR: " + stderr);
    console.log("(ifdown) STDOUT: " + stdout);

    console.log("\nCalling ifup wlan0");
    exec("ifup wlan0", (error, stdout, stderr) => {
      console.log("(ifup) ERROR: " + error);
      console.log("(ifup) STDERR: " + stderr);
      console.log("(ifup) STDOUT: " + stdout);

      // if (stdout != null) {
      //   console.log("(ifup) checking for assigned ip address");
      //
      //   const regex = /[\s\s]*?bound to (\d+\.\d+\.\d+\.\d+)[\s\s]*?/gi;
      //   if (stderr.match(regex)) {
      //     console.log("(ifup) ip address regex matched stdout.");
      //     return true;
      //   } else {
      //     console.log("(ifup) no ip address found in stdout.");
      //     return false;
      //   }
      // }
    });
  });
};
