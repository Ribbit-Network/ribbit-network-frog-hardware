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
        <Typography variant="h3">Now, let's test it.</Typography>

        <Typography variant="h5">
          Making sure every sensor is online and we can connect to the network.
        </Typography>
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
              setLoading(false);
            }, 400);
          }}
        >
          Next: Launch
        </LoadingButton>
      </Box>
    </Box>
  );
});
