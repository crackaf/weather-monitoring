import React from "react";
import { Route, Routes, BrowserRouter } from "react-router-dom";
import Home from "./pages/Home";
import Temperature from "./pages/Temperature";
import Humidity from "./pages/Humidity";
import AirQuality from "./pages/AirQuality";
import BarometericPressure from "./pages/BarometericPressure";

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/temperature" element={<Temperature />} />
        <Route path="/humidity" element={<Humidity />} />
        <Route path="/air-quality" element={<AirQuality />} />
        <Route path="/barometric-pressure" element={<BarometericPressure />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
