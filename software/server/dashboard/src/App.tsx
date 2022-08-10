import React from "react";
import {
  AppBar,
  Button,
  IconButton,
  Toolbar,
  Typography,
  Box,
  TextField,
  Select,
  Stepper,
  StepLabel,
  Step,
  StepContent,
} from "@mui/material";
import Authentication from "./pages/Authentication";
import { observer } from "mobx-react";
import Configuration from "./pages/Configuration";
import { onboardingCore } from "./core/OnboardingCore";
import TestFlight from "./pages/TestFlight";
import Finish from "./pages/Finish";

export default observer(() => {
  return (
    <Box sx={{ padding: 2 }}>
      <Stepper activeStep={onboardingCore.onboardingStep}>
        <Step key={0}>
          <StepLabel>Authentication 🥳</StepLabel>
        </Step>

        <Step key={1}>
          <StepLabel>Configuration ⚙️</StepLabel>
        </Step>

        <Step key={2}>
          <StepLabel>Test flight 🛫</StepLabel>
        </Step>

        <Step key={3}>
          <StepLabel>Launch 🚀</StepLabel>
        </Step>
      </Stepper>

      {onboardingCore.onboardingStep == 0 && <Authentication />}
      {onboardingCore.onboardingStep == 1 && <Configuration />}
      {onboardingCore.onboardingStep == 2 && <TestFlight />}
      {onboardingCore.onboardingStep == 3 && <Finish />}
    </Box>
  );
});
