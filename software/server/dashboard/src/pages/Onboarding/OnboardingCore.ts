import { Settings } from "../../core";
import { makeAutoObservable } from "mobx";
import { core } from "../../core";
import { userCore } from "../../core";
import axios from "axios";

const serverURL = "http://localhost:80";

class OnboardingCore {
  settings: Settings = {
    platformName: "",
    hostname: "",
    frequency: "",
  };

  onboardingStep = 0;

  testsComplete = false;

  // get onboardingStep() {
  //   // auth
  //   if (!userCore.activeUser) {
  //     return 0;
  //   }
  //
  //   // config
  //   if (
  //     (!core.settings?.platformName || !core.settings?.hostname) &&
  //     !core.settings?.uuid
  //   ) {
  //     return 1;
  //   }
  //
  //   // test
  //   if (core.settings?.uuid && !this.testsComplete) {
  //     return 2;
  //   }
  //
  //   if (this.testsComplete) {
  //     return 3;
  //   }
  // }

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

  completeTests() {
    this.testsComplete = true;
  }
}

export const onboardingCore = new OnboardingCore();
