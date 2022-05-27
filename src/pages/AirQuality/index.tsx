import React, { useEffect, useState } from "react";
import axios from "axios";
import styled from "styled-components";
import Card from "../../components/Card";
import InfoCard from "../../components/InfoCard";

const Container = styled.div`
  padding: 1%;
  display: flex;
  flex-direction: row;
  @media screen and (max-width: 768px) {
    flex-direction: column;
  }
`;

const LeftContainer = styled.div`
  width: 35%;
  height: 100%;
  @media screen and (max-width: 768px) {
    width: 100%;
  }
`;
const RightContainer = styled.div`
  width: 65%;
  height: 100%;
  @media screen and (max-width: 768px) {
    width: 100%;
  }
`;

const ChartContainer = styled.div`
  height: fit-content;
  width: fit-content;
  border-radius: 5px;
  margin: 5%;
  padding: 10px;
  box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
  :hover {
    box-shadow: rgba(0, 0, 0, 0.25) 0px 54px 55px,
      rgba(0, 0, 0, 0.12) 0px -12px 30px, rgba(0, 0, 0, 0.12) 0px 4px 6px,
      rgba(0, 0, 0, 0.17) 0px 12px 13px, rgba(0, 0, 0, 0.09) 0px -3px 5px;
  }
`;

function AirQuality() {
  const [airQuality, setAirQuality] = useState("0");
  useEffect(() => {
    axios
      .get("https://api.thingspeak.com/channels/1745289/feeds.json?results=2")
      .then(function (response) {
        console.log(response);
        console.log(response.data.feeds.length);
        setAirQuality(
          response.data.feeds[response.data.feeds.length - 1].field3
        );
      });
  }, []);
  return (
    <Container>
      <LeftContainer>
        <Card
          title="Air Quality"
          icon={1}
          value={airQuality}
          width="350px"
          height="250px"
        />
        <ChartContainer>
          <iframe
            title="Air Quality"
            width="450"
            height="260"
            src="https://thingspeak.com/channels/1745289/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"
          />
        </ChartContainer>
      </LeftContainer>
      <RightContainer>
        <InfoCard value={airQuality} width="850px" />
      </RightContainer>
    </Container>
  );
}

export default AirQuality;
