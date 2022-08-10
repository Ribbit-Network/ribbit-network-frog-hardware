import { Settings } from "./types";
import { makeAutoObservable } from "mobx";
import { core } from "./core";
import { userCore } from "./UserCore";
import axios from "axios";

const serverURL = "http://localhost:80";

class OnboardingCore {
  settings: Settings = {
    platformName: "",
    hostname: "",
    frequency: "",
  };

  get onboardingStep() {
    // auth
    if (!userCore.activeUser) {
      return 0;
    }

    // config
    if (!core.settings?.platformName || !core.settings?.hostname) {
      return 1;
    }

    // test
    if (core.settings?.platformName) {
      return 2;
    }
  }

  constructor() {
    makeAutoObservable(this);
  }

  async saveSettings() {
    const done = await axios.post(
      `${serverURL}/settings`,
      {
        ...this.settings,
      },
      {}
    );

    core.settings = this.settings;
  }
}

export const onboardingCore = new OnboardingCore();
