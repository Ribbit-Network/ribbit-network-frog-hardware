import React, { useEffect, useRef, useState } from "react";
import { Box, Button, TextField, Typography } from "@mui/material";
import { core, userCore } from "../core";
import { observer } from "mobx-react";
import "firebase/compat/auth";
import { onboardingCore } from "../core/OnboardingCore";
import { LoadingButton } from "@mui/lab";

export default observer(() => {
  const [loading, setLoading] = useState(false);

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
        <Typography variant="h3">
          Great, {userCore.userProfile?.displayName}
        </Typography>

        <Typography variant="h5">Let's configure your frog</Typography>
      </Box>

      <TextField
        sx={{
          mb: 2,
        }}
        name={"platform_name"}
        helperText={""}
        label={"Name it"}
        placeholder={"Backyard frog"}
        value={onboardingCore.settings.platformName}
        onChange={(e) => {
          onboardingCore.settings.platformName = e.target.value;
        }}
      />
      <TextField
        sx={{
          mb: 2,
        }}
        name={"hostname"}
        helperText={""}
        label={"Hostname"}
        placeholder={"frog.local"}
        value={onboardingCore.settings.hostname}
        onChange={(e) => {
          onboardingCore.settings.hostname = e.target.value;
        }}
      />

      <TextField
        type={"number"}
        sx={{
          mb: 2,
        }}
        name={"frequency"}
        helperText={
          "This is in 'times per day'. Put 24 to fetch the sensors every hour."
        }
        label={"Sensor reading frequency"}
        placeholder={"4 times a day"}
        value={onboardingCore.settings.frequency}
        onChange={(e) => {
          onboardingCore.settings.frequency = e.target.value;
        }}
      ></TextField>

      <Box sx={{ display: "flex", justifyContent: "space-between" }}>
        <LoadingButton loading={loading} variant={"outlined"}>
          Back: Authentication
        </LoadingButton>
        <LoadingButton
          loading={loading}
          variant={"contained"}
          onClick={() => {
            setLoading(true);

            onboardingCore.saveSettings().then(() => {
              setTimeout(() => {
                setLoading(false);
              }, 1000);
            });
          }}
        >
          Next: test
        </LoadingButton>
      </Box>
    </Box>
  );
});
