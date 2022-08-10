import React from "react";
import {
  AppBar,
  Button,
  IconButton,
  Toolbar,
  Typography,
  Box,
} from "@mui/material";
import { core } from "./core";
import Authentication from "./pages/Authentication";

function App() {
  core.heartbeat();

  if (!core.settings.onboarded) {
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
        <Box>
          <Typography variant="h3">Welcome to the Ribbit Network.</Typography>

          <Typography variant="h5">
            We're so glad you got here. Now that you have a running Frog, let's
            set you up in a few quick steps, and off we go measuring CO2 and
            saving the planet.
          </Typography>
        </Box>

        <Box
          sx={{
            display: "flex",
            justifyContent: "center",
          }}
        >
          <Authentication />
        </Box>
      </Box>
    );
  }
  return <Box />;
}

export default App;
