import React, { Component } from 'react';
import classes from "./Dashboard.module.css";
import { managerData, nationalAverageData, yearLabels } from "../data/mockData";
import { Line } from 'react-chartjs-2';
import { Button, Space } from 'antd';

const getLineColor = (n) => {
    switch (n) {
      case 's':
        return '#03c4a1';
      case 'a':
        return '#590995';
      case 'i':
        return '#c62a88';
      case 'v':
        return '#61b15a';
      case 'r':
        return '#150485';
      default:
        return '#33b2ff';
    }
}

const lineSettings = {
    // label: 'Test Dataset 1',
    fill: false,
    lineTension: 0.2,
    backgroundColor: 'rgba(75,192,192,0.4)',
    // borderColor: '#590995',
    borderCapStyle: 'butt',
    borderDash: [],
    borderDashOffset: 0.0,
    borderJoinStyle: 'miter',
    // pointBorderColor: '#590995',
    pointBackgroundColor: '#fff',
    pointBorderWidth: 1,
    pointHoverRadius: 5,
    pointHoverBackgroundColor: 'rgba(75,192,192,1)',
    pointHoverBorderColor: 'rgba(220,220,220,1)',
    pointHoverBorderWidth: 2,
    pointRadius: 1,
    pointHitRadius: 10,
    // data: this.props.data.data.s,
  }

export default class Dashboard extends Component {
    render() {
        console.log("plot props: ", this.props.data);
        const newData = {
            labels: this.props.data.labels,
            datasets: Object.keys(this.props.data.data).map(key => {
                const color = getLineColor(key);
                return {
                    ...lineSettings,
                    label: key,
                    borderColor: color,
                    pointBorderColor: color,
                    data: this.props.data.data[key],
                }
            })
        }
        return (
            <div className={classes.container}>
                <Space direction="vertical">
                    <div className={classes.graphContainer}>
                        <Line
                            data={newData}
                        />
                    </div>
                    {/* <Button onClick={this.addChartData}>Add Data 1</Button> */}
                </Space>
            </div>
        )
    }
}