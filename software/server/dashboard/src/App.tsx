import React from "react";
import "./App.css";
import {
  AppBar,
  Button,
  IconButton,
  Toolbar,
  Typography,
  Box,
} from "@mui/material";
import frog from "./frog.svg";
import core from "./core/core";

function App() {
  return (
    <Box
      sx={{
        display: "flex",
        flexDirection: "column",
        justifyContent: "space-between",
        alignItems: "space-between",

        height: "100%",
      }}
    >
      <Box>
        <Typography variant="h3">Welcome to the Ribbit Network.</Typography>

        <Typography variant="h5">
          We're so glad you got here. Let's set you up in a few quick steps, and
          off we go measuring CO2 and saving the planet.
        </Typography>
      </Box>

      <Box>
        <Button sx={{ mr: 2 }} variant={"contained"}>
          Login
        </Button>
        <Button variant={"contained"}>Sign Up</Button>
      </Box>
    </Box>
  );
}

export default App;
