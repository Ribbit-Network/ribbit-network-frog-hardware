import React from "react";
import "./App.css";

import { AppBar, Button, IconButton, Toolbar, Typography } from "@mui/material";
import frog from "./frog.svg";
import MyMapComponent from "./components/Map";

function App() {
  return (
    <>
      <AppBar color={"default"}>
        <Toolbar color={"transparent"}>
          <IconButton
            size="large"
            edge="start"
            color="inherit"
            aria-label="menu"
            sx={{ mr: 2 }}
          >
            <img width={48} src={frog} />
          </IconButton>
          <Typography variant="h6" component="div" sx={{ flexGrow: 1 }}>
            Welcome to Ribbit Network!
          </Typography>

          <Button color="inherit" href={"https://ko-fi.com/keenanjohnson"}>
            Support
          </Button>
        </Toolbar>
      </AppBar>

      <Typography></Typography>
    </>
  );
}

export default App;
