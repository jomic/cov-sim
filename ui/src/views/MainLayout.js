import React, { useEffect, useRef, useState } from 'react';
import 'antd/dist/antd.css';
import classes from './MainLayout.module.css';
import { Layout, Menu, Breadcrumb, Card, Space, Button, Switch} from 'antd';
import { UserOutlined, LaptopOutlined, NotificationOutlined } from '@ant-design/icons';
import ForceGraph2D from 'react-force-graph-2d';
import Dashboard from './Dashboard';
import SimulationForm from '../components/SimulationForm';

// https://medium.com/@jjsincorporated/how-i-built-conways-game-of-life-with-react-hooks-3bc6c2734aa

const { SubMenu } = Menu;
const { Header, Content, Sider } = Layout;
var testData = require('../data/test.json'); //with path
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
  
function genRandomTree(N = 300, reverse = false) {
return {
    nodes: [...Array(N).keys()].map(i => ({ id: i })),
    links: [...Array(N).keys()]
    .filter(id => id)
    .map(id => ({
    [reverse ? 'target' : 'source']: id,
    [reverse ? 'source' : 'target']: Math.round(Math.random() * (id-1)/2)
    }))
};
}

var randomData = genRandomTree(100, false);


function MainLayout() {
    const graphDataRef = useRef({});
    const [plotData, setPlotData] = useState({
        data: {
            s: [],
            a: [],
            i: [],
            v: [],
            r: [],
        },
        labels: []
    });


    async function fetchData(endpoint, method, reset) {
        const request = await fetch(endpoint, {method: method});
        const response = await request.json();
        console.log("response: ", response);
        if (reset) {
            graphDataRef.current.nodes = undefined;
            graphDataRef.current.links = undefined;
        }
        updateData(response);
    }

    useEffect(async () => {
        fetchData("/api/simulation", "post");
    }, []);

    const updateData = (response) => {
        const state = {...plotData};
        if (!graphDataRef.current.nodes) {
            graphDataRef.current = response.graph;
        } else {
            for (let i = 0; i < graphDataRef.current.nodes.length; i++) {
                graphDataRef.current.nodes[i].state = response.graph.nodes[i].state;
            }
        }

        const plot = response.plot;
        state.data.s = [...state.data.s, plot.s];
        state.data.a = [...state.data.a, plot.a];
        state.data.i = [...state.data.i, plot.i];
        state.data.v = [...state.data.v, plot.v];
        state.data.r = [...state.data.r, plot.r];
        state.labels = [...state.labels, state.labels.length];
        setPlotData(state);
    }

    const requestData = async () => {
        const request = await fetch("/api/simulation", {method: 'put'});
        const response = await request.json();
        updateData(response);   
    }

    return(
        <Layout>
            <Header className="header" style={{ position: 'fixed', zIndex: 1, width: '100%' }}>
                <h2 style={{
                    marginTop: '8px', 
                    color: 'white', 
                    fontWeight: '250',
                    margin: '8px 10px 10px -25px',
            
                    }}>Covid-19 Simulator</h2>
                <Menu theme="dark" mode="horizontal"
                defaultSelectedKeys={['simulator']}>
                    {/* <Menu.Item key="1">Home</Menu.Item> */}
                    <Menu.Item key="simulator">Simulator</Menu.Item>
                    <Menu.Item key="about">About</Menu.Item>
                </Menu>
            </Header>
            <Layout>
                <Layout style={{ padding: '64px 24px 24px'}} >
                        <Breadcrumb style={{ margin: '16px 0', textAlign: "left"}}>
                            <Breadcrumb.Item>Simulator</Breadcrumb.Item>
                            <Breadcrumb.Item>Graph-based</Breadcrumb.Item>
                            {/* <Breadcrumb.Item>Simulation</Breadcrumb.Item> */}
                        </Breadcrumb>
                        <Content   
                            className="site-layout-background"
                            style={{
                                padding: 24,
                                margin: 0,
                                minHeight: 280,
                            }}
                        >
                            <Space direction="horizontal">
                                <Card bordered>
                                    <h3>Agents Graph</h3>
                                    {graphDataRef.current.nodes && (
                                        <ForceGraph2D 
                                            height={500}
                                            width={800}
                                            graphData={graphDataRef.current} 
                                            enableZoomPanInteraction={true}
                                            nodeLabel="state"
                                            nodeCanvasObject={({ state, x, y }, ctx) => {
                                                ctx.fillStyle = getStateColor(state);
                                                ctx.beginPath(); ctx.arc(x, y, 5, 0, 2 * Math.PI, false); ctx.fill(); // circle
                                            }}
                                        />
                                    )}
                                    <h3>Plot</h3>
                                    <Dashboard data={plotData}/>     
                                </Card>
                            </Space>                            
                        </Content>
                    </Layout>
                <Sider 
                    width={300} 
                    theme="light"
                    className="site-layout-background"
                    style={{
                        overflow: 'auto',
                        // overflowY: 'auto',
                        height: '100vh',
                        position: 'fixed',
                        right: 0,
                        // top: "64px",
                        // paddingBottom: "64px",
                        paddingTop: "64px",
                      }}>
                    <SimulationForm fetchData={fetchData}/>
                    <Button onClick={requestData} style={{marginLeft: "78px"}}>Request Data</Button>
                </Sider>
            </Layout>
        </Layout>
    )
}

export default MainLayout;