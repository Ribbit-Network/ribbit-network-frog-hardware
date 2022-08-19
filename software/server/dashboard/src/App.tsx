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
import Authentication from "./pages/Onboarding/Authentication";
import { observer } from "mobx-react";
import Configuration from "./pages/Onboarding/Configuration";
import { onboardingCore } from "./pages/Onboarding/OnboardingCore";
import TestFlight from "./pages/Onboarding/TestFlight";
import Finish from "./pages/Onboarding/Finish";
import Onboarding from "./pages/Onboarding";

export default observer(() => {
  return (
    <Box sx={{ padding: 0, height: "100%" }}>
      <Onboarding />
    </Box>
  );
});
