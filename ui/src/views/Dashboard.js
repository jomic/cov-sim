import React, { Component } from 'react';
import classes from "./Dashboard.module.css";
import { managerData, nationalAverageData, yearLabels } from "../data/mockData";
import { Line } from 'react-chartjs-2';
import { Button, Space } from 'antd';


export default class Dashboard extends Component {
    state = {
        data: managerData,
        average: nationalAverageData,
        labels: yearLabels,
    }

    graphRef = React.createRef();

    addChartData = () => {
        // this.graphRef.current.addData();
        const newData = [...this.state.data, 4000];
        const newLabels = [...this.state.labels, this.state.labels.length];
        this.setState({
            data: newData,
            labels: newLabels,
        });
    }



    render() {
        const { data, average, labels } = this.state;
        const newData = {
            labels: this.state.labels,
            datasets: [
              {
                label: 'Test Dataset 1',
                fill: false,
                lineTension: 0.2,
                backgroundColor: 'rgba(75,192,192,0.4)',
                borderColor: '#590995',
                borderCapStyle: 'butt',
                borderDash: [],
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: '#590995',
                pointBackgroundColor: '#fff',
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: 'rgba(75,192,192,1)',
                pointHoverBorderColor: 'rgba(220,220,220,1)',
                pointHoverBorderWidth: 2,
                pointRadius: 1,
                pointHitRadius: 10,
                data: this.state.data,
              }
            ]
        }
        return (
            <div className={classes.container}>
                <Space direction="vertical">
                    <div className={classes.graphContainer}>
                        <Line
                            data={newData}
                        />
                    </div>
                    <Button onClick={this.addChartData}>Add Data 1</Button>
                </Space>
            </div>
        )
    }
}