import './App.css';
import React, { useCallback, useEffect, useRef, useState } from 'react';
import ForceGraph2D from 'react-force-graph-2d';
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

  var randomData = genRandomTree(500, false);
  const getColor = n => '#' + ((n * 1234567) % Math.pow(2, 24)).toString(16).padStart(6, '0');
  
  // states: 0 - susceptible, 1 - asymptomatic, 2 - infected, 3 - vaccinated, 4 - rehabed
  const getStateColor = (n) => {
    switch (n) {
      case 0:
        return '#03c4a1';
      case 1:
        return '#590995';
      case 2:
        return '#c62a88';
      case 3:
        return '#61b15a';
      case 4:
        return '#150485';
      default:
        return '#33b2ff';
    }
  } 
  console.log(testData);
  const { useRef } = React;
  const fgRef = useRef();

  return (
    <div className="App">
      <section className="Main" >
        <h3>Test Data (with states)</h3>
        <ForceGraph2D 
          graphData={testData} 
          enableZoomPanInteraction={false}
          nodeLabel="state"
          nodeCanvasObject={({ state, x, y }, ctx) => {
            ctx.fillStyle = getStateColor(state);
            ctx.beginPath(); ctx.arc(x, y, 5, 0, 2 * Math.PI, false); ctx.fill(); // circle
          }}
        />
        <h3>Randomly Generated Data</h3>
        <ForceGraph2D 
          ref={fgRef}
          graphData={randomData} 
          enableZoomPanInteraction={false}
          nodeLabel="state"
          nodeAutoColorBy={d => d.id%5}
          onEngineStop={() => fgRef.current.zoomToFit(400)}
        />
        <Dashboard />
      </section>
    </div>
  );
}

export default App;
