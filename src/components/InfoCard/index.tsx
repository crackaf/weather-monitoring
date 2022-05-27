import React from "react";
import styled from "styled-components";

const CardContainer = styled.div<{ width?: string; height?: string }>`
  border-radius: 5px;
  margin: 5%;
  padding: 10px;
  text-align: center;
  box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
  max-width: ${({ width }) => width ?? "300px"};
  height: ${({ height }) => height ?? "200px"};
`;

const ChartContainer = styled.div<{ chartUrl?: string }>`
  width: 100%;
  height: 100%;
  background-image: url(${({ chartUrl }) => chartUrl});
  background-size: contain;
  background-repeat: no-repeat;
`;

function InfoCard({
  value,
  chartUrl,
  width,
  height,
}: {
  value: string;
  chartUrl?: string;
  width?: string;
  height?: string;
}) {
  return (
    <CardContainer width={width} height={height}>
      <div>
        <h1>Current Status</h1>
        <h3>{value}</h3>
      </div>
      <hr />
      <ChartContainer chartUrl={chartUrl} />
    </CardContainer>
  );
}

export default InfoCard;
