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
  Grid,
  Paper,
} from "@mui/material";
import { observer } from "mobx-react";
import { onboardingCore } from "./OnboardingCore";
import Authentication from "./Authentication";
import Configuration from "./Configuration";
import TestFlight from "./TestFlight";
import Finish from "./Finish";
import Welcome from "./Welcome";

export default observer(() => {
  return (
    <Grid
      sx={{
        backgroundColor: "darkcyan",
        height: "100%",
      }}
      container
      justifyContent={"center"}
      alignContent={"center"}
    >
      <Grid xs={12} sm={8} md={6} lg={4} item>
        <Paper sx={{ padding: 2 }}>
          <Stepper activeStep={onboardingCore.onboardingStep}>
            <Step key={0}>
              <StepLabel>Welcome 🥳</StepLabel>
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

          {onboardingCore.onboardingStep == 0 && <Welcome />}
          {onboardingCore.onboardingStep == 1 && <Configuration />}
          {onboardingCore.onboardingStep == 2 && <TestFlight />}
          {onboardingCore.onboardingStep == 3 && <Finish />}
        </Paper>
      </Grid>
    </Grid>
  );
});
