import React, { PureComponent } from 'react';
import Chart from 'chart.js';
import classes from "./LineGraph.module.css";
let LineChart;
export default class LineGraph extends PureComponent {
    chartRef = React.createRef();

    componentDidMount() {
        this.buildChart();
    }

    componentDidUpdate() {
        this.buildChart();
    }

    addData = () => {
        console.log("Adding data");
    }

    buildChart = () => {
        const myChartRef = this.chartRef.current.getContext("2d");
        const { data, average, labels } = this.props;

        if (typeof LineChart !== "undefined") LineChart.destroy();

        LineChart = new Chart(myChartRef, {
            type: "line",
            data: {
                labels: labels,
                datasets: [
                    {
                        label: "Sales",
                        data: data,
                        fill: false,
                        borderColor: "#6610f2"
                    },
                    {
                        label: "National Average",
                        data: average,
                        fill: false,
                        borderColor: "#E0E0E0"
                    }
                ]
            },
            options: {
                //customize
                responsive: true,
                maintainAspectRatio: false,
            }
        });
    }

    render() {
        console.log("Graph Updated", this.props.data);
        
        return (
            <div className={classes.graphContainer}>
                <canvas
                    id="myChart"
                    ref={this.chartRef} 
                />
            </div>
        )
    }
}