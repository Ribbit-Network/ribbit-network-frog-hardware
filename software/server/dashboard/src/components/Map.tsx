import React, { useEffect } from "react";
import { Box } from "@mui/material";
import core from "../core/core";
import { observer } from "mobx-react";

export default observer(() => {
  const ref = React.useRef<HTMLDivElement>(null);

  useEffect(() => {
    if (!core.sensorData.length) {
      return;
    }

    const mapInstance = new window.google.maps.Map(ref.current!, {
      zoom: 3,
      center: { lat: 37.775, lng: -122.434 },
      mapTypeId: "satellite",
    });

    const heatmapRows = core.sensorData.map((row) => {
      const point = new google.maps.LatLng(row.lat, row.lon);
      const infoWindow = new google.maps.InfoWindow({
        content: `
        <div>
              <Typography><strong>${row.co2.toFixed(
                0
              )}</strong> kg CO2 emitted</Typography>
              <br/>
              <Typography>${new Date(row._time).toDateString()}</Typography>
            </div>`,
      });

      const marker = new google.maps.Marker({
        position: point,
        map: mapInstance,
        title: "",
        icon: " ",
      });

      marker.addListener("click", () => {
        infoWindow.open({
          anchor: marker,
          map: mapInstance,
          shouldFocus: false,
        });
      });

      return point;
    });

    const heatmap = new window.google.maps.visualization.HeatmapLayer({
      data: heatmapRows,
      map: mapInstance,
      radius: 25,
    });

    heatmap.setMap(mapInstance);
  }, [core.sensorData]);

  return (
    <Box
      sx={{
        height: "100vh",
        width: "100vw",
      }}
      ref={ref}
      id="map"
    />
  );
});
