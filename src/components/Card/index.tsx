import React from "react";
import { useNavigate } from "react-router-dom";
import styled from "styled-components";
import ThermostatIcon from "@mui/icons-material/Thermostat";
import AirIcon from "@mui/icons-material/Air";
import OpacityIcon from "@mui/icons-material/Opacity";
import CompressIcon from "@mui/icons-material/Compress";

const CardContainer = styled.div<{ width?: string; height?: string }>`
  border-radius: 5px;
  margin: 5%;
  padding: 10px;
  text-align: center;
  box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
  width: ${({ width }) => width ?? "300px"};
  height: ${({ height }) => height ?? "200px"};
  :hover {
    cursor: pointer;
    box-shadow: rgba(0, 0, 0, 0.25) 0px 54px 55px,
      rgba(0, 0, 0, 0.12) 0px -12px 30px, rgba(0, 0, 0, 0.12) 0px 4px 6px,
      rgba(0, 0, 0, 0.17) 0px 12px 13px, rgba(0, 0, 0, 0.09) 0px -3px 5px;
  }
`;

const Upper = styled.div`
  height: 50%;
  width: 100%;
`;

const Lower = styled.div`
  height: 50%;
  width: 100%;
`;

const Title = styled.div`
  font-size: 30px;
`;

function getIcon(icon: number): any {
  if (icon === 1) {
    return <ThermostatIcon fontSize="large" />;
  }
  if (icon === 2) {
    return <AirIcon fontSize="large" />;
  }
  if (icon === 3) {
    return <OpacityIcon fontSize="large" />;
  }
  if (icon === 4) {
    return <CompressIcon fontSize="large" />;
  }
}

function handleClick(navigate: any, navigateLink: string | undefined) {
  if (!!navigateLink) {
    navigate(navigateLink);
  }
}

function Card({
  title,
  navigateLink,
  icon,
  value,
  width,
  height,
}: {
  title: string;
  navigateLink?: string;
  icon: number;
  value: string;
  width?: string;
  height?: string;
}) {
  const navigate = useNavigate();

  return (
    <CardContainer
      width={width}
      height={height}
      onClick={() => handleClick(navigate, navigateLink)}
    >
      <Upper>
        <div>{getIcon(icon)}</div>
        <Title>{title}</Title>
      </Upper>
      <hr />
      <Lower>
        <h4>{value}</h4>
      </Lower>
    </CardContainer>
  );
}

export default Card;
