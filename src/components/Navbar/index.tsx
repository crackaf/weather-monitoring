import React from "react";
import "./style.css";

function Navbar() {
  return (
    <nav className="navbar navbar-expand-lg navbar-light bg-light">
      <div className="container-fluid">
        <a className="navbar-brand" href="/">
          Weather Monitoring System
        </a>
      </div>
    </nav>
  );
}

export default Navbar;
