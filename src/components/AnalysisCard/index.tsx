import React from "react";
import styled from "styled-components";

const CardContainer = styled.div<{ width?: string; height?: string }>`
  border-radius: 5px;
  margin: 5%;
  padding: 10px;
  text-align: center;
  box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
  width: ${({ width }) => width ?? "300px"};
  height: ${({ height }) => height ?? "200px"};
`;

function AnalysisCard({ width, height }: { width?: string; height?: string }) {
  return <CardContainer width={width} height={height}></CardContainer>;
}

export default AnalysisCard;
