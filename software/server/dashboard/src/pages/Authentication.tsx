import React, { useEffect } from "react";
import { Box } from "@mui/material";
import { core } from "../core";
import { observer } from "mobx-react";
import * as firebaseui from "firebaseui";
import firebaseAuth from "firebase/compat/app";
import "firebase/compat/auth";

const uiConfig = {
  // Popup signin flow rather than redirect flow.
  signInFlow: "popup",
  // Redirect to /signedIn after sign in is successful. Alternatively you can provide a callbacks.signInSuccess function.
  signInSuccessUrl: "/signedIn",

  signInOptions: [
    firebaseAuth.auth.GoogleAuthProvider.PROVIDER_ID,
    firebaseAuth.auth.EmailAuthProvider.PROVIDER_ID,
    firebaseui.auth.AnonymousAuthProvider.PROVIDER_ID,
  ],
};

export default observer(() => {
  useEffect(() => {
    if (!core.authUI) {
      core.authUI = new firebaseui.auth.AuthUI(core.auth);
    }

    core.authUI.start("#firebaseui-auth-container", uiConfig);
  }, []);
  return <div id="firebaseui-auth-container"></div>;
});
