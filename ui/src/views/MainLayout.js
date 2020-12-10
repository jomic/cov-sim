import React from 'react';
import 'antd/dist/antd.css';
import './MainLayout.module.css';
import { Layout, Menu, Breadcrumb, Card, Space, Button, Switch} from 'antd';
import { UserOutlined, LaptopOutlined, NotificationOutlined } from '@ant-design/icons';
import ForceGraph2D from 'react-force-graph-2d';
import Dashboard from './Dashboard';

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
    return(
        <Layout>
            <Header className="header">
                <h2 style={{
                    marginTop: '8px', 
                    color: 'white', 
                    fontWeight: '250',
                    margin: '8px 10px 10px -25px',
                    }}>Covid-19 Simulator</h2>
                <Menu theme="dark" mode="horizontal"
                defaultSelectedKeys={['2']}>
                    <Menu.Item key="1">Home</Menu.Item>
                    <Menu.Item key="2">Simulator</Menu.Item>
                    <Menu.Item key="3">About</Menu.Item>
                </Menu>
                <Switch
                    checkedChildren='+'
                    unCheckedChildren='-'
                    defaultChecked
                    style={{position: 'absolute', right: '12px'}}
                />
            </Header>
            <Layout>
                <Layout style={{ padding: '0 24px 24px'}}>
                        <Breadcrumb style={{ margin: '16px 0', textAlign: "left"}}>
                            <Breadcrumb.Item>Home</Breadcrumb.Item>
                            <Breadcrumb.Item>Graph-based</Breadcrumb.Item>
                            <Breadcrumb.Item>Simulation</Breadcrumb.Item>
                        </Breadcrumb>
                        <Content   
                            className="site-layout-background"
                            style={{
                                padding: 24,
                                margin: 0,
                                minHeight: 280,
                            }}
                        >
                            <Space direction="vertical">
                                <Card bordered>
                                    <h3>Test Data (with states)</h3>
                                    <ForceGraph2D 
                                        height='500'
                                        width='800'
                                        graphData={testData} 
                                        enableZoomPanInteraction={false}
                                        nodeLabel="state"
                                        nodeCanvasObject={({ state, x, y }, ctx) => {
                                            ctx.fillStyle = getStateColor(state);
                                            ctx.beginPath(); ctx.arc(x, y, 5, 0, 2 * Math.PI, false); ctx.fill(); // circle
                                        }}
                                    />
                                </Card>
                                <Card>
                                    <h3>Randomly Generated Data</h3>
                                    <ForceGraph2D 
                                        height='500'
                                        width='800'
                                        graphData={randomData} 
                                        enableZoomPanInteraction={false}
                                        nodeLabel="state"
                                        nodeAutoColorBy={d => d.id%5}
                                    />
                                </Card>
                                <Card bordered>
                                    <h3>Plot</h3>
                                    <Dashboard />
                                </Card>
                            </Space>                            
                        </Content>
                    </Layout>
                <Sider width={300} className="site-layout-background">
                    <Menu
                        mode="inline"
                        defaultSelectedKeys={['1']}
                        defaultOpenKeys={['sub1']}
                        style={{ height: '100%', borderRight: 0}}
                    >
                        <SubMenu key="sub1" icon={<UserOutlined />} title="subnav1">
                            <Menu.Item key="1">option1</Menu.Item>
                            <Menu.Item key="2">option2</Menu.Item>
                            <Menu.Item key="3">option3</Menu.Item>
                            <Menu.Item key="4">option4</Menu.Item>
                        </SubMenu>
                        <SubMenu key="sub2" icon={<LaptopOutlined />} title="subnav2">
                            <Menu.Item key="5">option1</Menu.Item>
                            <Menu.Item key="6">option2</Menu.Item>
                            <Menu.Item key="7">option3</Menu.Item>
                            <Menu.Item key="8">option4</Menu.Item>
                        </SubMenu>
                        <Space style={{float: 'right', padding: '8px'}}>
                            <Button type="primary">Save</Button>
                            <Button>Reset</Button>        
                        </Space>

                    </Menu>
                </Sider>
            </Layout>
        </Layout>
    )
}

export default MainLayout;