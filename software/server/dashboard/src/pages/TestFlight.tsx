import React, { useEffect, useRef, useState } from "react";
import {
  Box,
  Button,
  CircularProgress,
  TextField,
  Tooltip,
  Typography,
} from "@mui/material";
import { core, userCore } from "../core";
import { observer } from "mobx-react";
import "firebase/compat/auth";
import { onboardingCore } from "../core/OnboardingCore";
import { LoadingButton } from "@mui/lab";
import { Check, Error } from "@mui/icons-material";
import { makeAutoObservable } from "mobx";

enum State {
  loading,
  error,
  good,
}

class TestFlight {
  connectionToLocalDB = State.loading;
  connectionToRibbitNetwork = State.loading;
  gpsSensor = State.loading;
  co2Sensor = State.loading;
  barometry = State.loading;

  constructor() {
    makeAutoObservable(this);

    setTimeout(() => {
      this.testLocalDB();
      this.testRibbitNetworkConnection();
      this.testGPS();
      this.testCO2();
      this.testBarometry();
    }, 500);
  }

  testLocalDB() {
    this.connectionToLocalDB = State.good;
  }

  testRibbitNetworkConnection() {
    this.connectionToRibbitNetwork = State.good;
  }

  testGPS() {
    this.gpsSensor = State.good;
  }

  testCO2() {
    this.co2Sensor = State.error;
  }

  testBarometry() {
    this.barometry = State.error;
  }
}

const testFlight = new TestFlight();

export default observer(() => {
  const [loading, setLoading] = useState(false);

  const indicator = (property: State) => {
    if (property === State.loading) {
      return <CircularProgress size={20} />;
    }

    if (property === State.good) {
      return <Check />;
    }

    if (property === State.error) {
      return (
        <Tooltip
          title={"Something went wrong. Here are further instructions..."}
        >
          <Error />
        </Tooltip>
      );
    }
  };

  return (
    <Box
      sx={{
        display: "flex",
        flexDirection: "column",
        justifyContent: "space-between",
        alignItems: "space-between",
        padding: 4,
      }}
    >
      <Box sx={{ mb: 2 }}>
        <Typography variant="h3">Now, let's test it.</Typography>

        <Typography variant="h5">
          Making sure every sensor and connector is online.
        </Typography>
      </Box>

      <Box
        sx={{
          display: "flex",
          mb: 1,
        }}
      >
        {indicator(testFlight.connectionToLocalDB)}

        <Typography>Connection to local database</Typography>
      </Box>

      <Box
        sx={{
          display: "flex",
          mb: 1,
        }}
      >
        {indicator(testFlight.connectionToRibbitNetwork)}
        <Typography>Connection to Ribbit Network</Typography>
      </Box>

      <Box
        sx={{
          display: "flex",
          mb: 1,
        }}
      >
        {indicator(testFlight.gpsSensor)}
        <Typography>GPS sensor</Typography>
      </Box>

      <Box
        sx={{
          display: "flex",
          mb: 1,
        }}
      >
        {indicator(testFlight.co2Sensor)}
        <Typography>CO2 sensor</Typography>
      </Box>

      <Box
        sx={{
          display: "flex",
          mb: 1,
        }}
      >
        {indicator(testFlight.barometry)}
        <Typography>Barometric sensor</Typography>
      </Box>

      <Box sx={{ display: "flex", justifyContent: "space-between" }}>
        <LoadingButton loading={loading} variant={"outlined"}>
          Back: Configuration
        </LoadingButton>
        <LoadingButton
          loading={loading}
          variant={"contained"}
          onClick={() => {
            setLoading(true);

            setTimeout(() => {
              onboardingCore.completeTests();
              setLoading(false);
            }, 1000);
          }}
        >
          Next: Launch
        </LoadingButton>
      </Box>
    </Box>
  );
});
