import React, { useEffect, useRef, useState } from "react";
import { Box, Button, TextField, Typography } from "@mui/material";
import { core, userCore } from "../../core";
import { observer } from "mobx-react";
import "firebase/compat/auth";
import { onboardingCore } from "./OnboardingCore";
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
      <Box sx={{ mb: 12 }}>
        <Typography variant="h3">Welcome</Typography>

        <Typography variant="h5">Let's configure your frog</Typography>
      </Box>

      <Box sx={{ display: "flex", justifyContent: "center" }}>
        <LoadingButton
          loading={loading}
          variant={"contained"}
          onClick={() => {
            setLoading(true);

            onboardingCore.saveSettings().then(() => {
              setTimeout(() => {
                onboardingCore.onboardingStep++;
                setLoading(false);
              }, 1000);
            });
          }}
        >
          Next
        </LoadingButton>
      </Box>
    </Box>
  );
});
