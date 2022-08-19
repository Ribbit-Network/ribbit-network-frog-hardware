import React, { useEffect, useRef, useState } from "react";
import {
  Box,
  Button,
  CircularProgress,
  TextField,
  Tooltip,
  Typography,
} from "@mui/material";
import { core, userCore } from "../../core";
import { observer } from "mobx-react";
import "firebase/compat/auth";
import { LoadingButton } from "@mui/lab";
import { onboardingCore } from "./OnboardingCore";

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
        <Typography variant="h3">You're done.</Typography>

        <Typography variant="h5">Let's get it started.</Typography>
      </Box>

      <Box sx={{ display: "flex", justifyContent: "center" }}>
        <LoadingButton
          loading={loading}
          variant={"contained"}
          onClick={() => {
            setLoading(true);

            setTimeout(() => {
              userCore.doLogout();
              onboardingCore.onboardingStep = 0;
              setLoading(false);
            }, 1000);
          }}
        >
          Finish: Go to dashboard
        </LoadingButton>
      </Box>
    </Box>
  );
});
