import React, { useEffect, useState } from "react";
import axios from "axios";
import styled from "styled-components";
import Navbar from "../../components/Navbar";
import Card from "../../components/Card";

const Container = styled.div`
  padding-top: 5%;
  padding-left: 25%;
  padding-right: 20%;
`;

const CardContainer = styled.div`
  width: 100%;
  height: fit-content;
  display: flex;
  flex-wrap: wrap;
`;

function Home() {
  const [temperature, setTemperature] = useState("0");
  const [humidity, setHumidity] = useState("0");
  const [airQuality, setAirQuality] = useState("0");
  const [pressure, setPressure] = useState("0");
  useEffect(() => {
    axios
      .get("https://api.thingspeak.com/channels/1745289/feeds.json?results=2")
      .then(function (response) {
        console.log(response);
        console.log(response.data.feeds.length);
        const len = response.data.feeds.length - 1;
        setTemperature(response.data.feeds[len].field1);
        setHumidity(response.data.feeds[len].field2);
        setAirQuality(response.data.feeds[len].field3);
        setPressure(response.data.feeds[len].field4);
      });
  }, []);

  return (
    <>
      <Navbar />
      <Container>
        <CardContainer>
          <Card
            title="Temperature"
            navigateLink="/temperature"
            icon={1}
            value={temperature}
          />
          <Card
            title="Air Quality"
            navigateLink="/air-quality"
            icon={2}
            value={airQuality}
          />
          <Card
            title="Humidity"
            navigateLink="/humidity"
            icon={3}
            value={humidity}
          />
          <Card
            title="Barometric Pressure"
            navigateLink="/barometric-pressure"
            icon={4}
            value={pressure}
          />
        </CardContainer>
      </Container>
    </>
  );
}

export default Home;
