import './App.css';
import React, { useCallback, useEffect, useRef, useState } from 'react';
import Circle from './Circle.js';
import * as d3 from 'd3';
import BarChart from './BarChart';
import { svg } from 'd3';
import data from './data/data.json';
import ForceGraph2D from 'react-force-graph-2d';
import LineGraph from './components/LineGraph';
import Dashboard from './views/Dashboard';
function App() {

  function genRandomTree(N = 300, reverse = false) {
    return {
      nodes: [...Array(N).keys()].map(i => ({ id: i })),
        links: [...Array(N).keys()]
      .filter(id => id)
      .map(id => ({
        [reverse ? 'target' : 'source']: id,
        [reverse ? 'source' : 'target']: Math.round(Math.random() * (id-1))
      }))
    };
  }

  var myData = require('./data/miserables.json'); //with path
  var testData = require('./data/test.json'); //with path

  var randomData = genRandomTree(30, false);
  const getColor = n => '#' + ((n * 1234567) % Math.pow(2, 24)).toString(16).padStart(6, '0');
  
  // states: 0 - susceptible, 1 - asymptomatic, 2 - infected, 3 - vaccinated, 4 - rehabed
  const getStateColor = (n) => {
    switch (n) {
      case 0:
        return '#e4ff33';
      case 1:
        return '#FFC300';
      case 2:
        return '#C70039';
      case 3:
        return '#DAF7A6';
      case 4:
        return '#581845';
      default:
        return '#33b2ff';
    }
  } 
  console.log(testData);

  return (
    <div className="App">
      <section className="Main">
        <ForceGraph2D 
          graphData={testData} 
          enableZoomPanInteraction={false}
          nodeLabel="state"
          nodeCanvasObject={({ state, x, y }, ctx) => {
            ctx.fillStyle = getStateColor(state);
            ctx.beginPath(); ctx.arc(x, y, 5, 0, 2 * Math.PI, false); ctx.fill(); // circle
          }}
        />
        {/* <LineGraph /> */}
        <Dashboard />
      </section>
    </div>
  );
}

export default App;
