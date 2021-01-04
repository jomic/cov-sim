import React, { useState } from 'react';
import { Button, Collapse, Input, Radio, Select, Slider, Switch } from 'antd';
import Form from 'antd/lib/form/Form';
import FormItem from 'antd/lib/form/FormItem';
import FormList from 'antd/lib/form/FormList';
import { MinusCircleOutlined, PlusOutlined } from '@ant-design/icons';
const { Panel } = Collapse;
const { Option } = Select;
const text = 'testing the collapse panels';
const layout = {
    labelCol: {
      span: 8,
    },
    wrapperCol: {
      span: 16,
    },
  };
  const tailLayout = {
    wrapperCol: {
      offset: 8,
      span: 16,
    },
  };


  const formItemLayout = {
    labelCol: {
        span: 4,
    },
    wrapperCol: {
        span: 24
    },
  };
  const formItemLayoutWithOutLabel = {
    wrapperCol: {
      xs: { span: 24, offset: 4 },
      sm: { span: 20, offset: 4 },
    },
  };
function Region({fields, field, remove}) {
    return(
        <Collapse>
            <Panel 
                header={`Region ${field.key}`} 
                key={field.key} 
                extra={
                    fields.length > 1 ? (
                    <MinusCircleOutlined
                    className="dynamic-delete-button"
                    onClick={() => remove(field.name)} 
                    />) : null
                    }
            >
                <FormItem
                    {...formItemLayout}
                    // {...(index === 0 ? formItemLayout : formItemLayoutWithOutLabel)}
                    // label={index === 0 ? 'Label' : ''}
                    required={false}
                    key={field.key}
                    noStyle
                >
                    <FormItem
                        {...field}
                        label="Type"
                        labelCol={{span: 8}}
                        wrapperCol={{span:20}}                        
                        name={[field.name, 'graph_type']}
                        fieldKey={[field.fieldKey, 'graph_type']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Radio.Group>
                            <Radio.Button value="nw_small_world">Small World</Radio.Button>
                            <Radio.Button value="matrix">Matrix</Radio.Button>

                        </Radio.Group>
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Population"
                        labelCol={{span: 8}}
                        wrapperCol={{span:20}}
                        name={[field.name, 'population']}
                        fieldKey={[field.fieldKey, 'population']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="region population" style={{width: '80%'}} />
                    </FormItem>

                    <FormItem

                        name={[field.name, 'connections']}
                        fieldKey={[field.fieldKey, 'connections']}
                        label="Connected to"
                        rules={[
                        {
                            required: false,
                            message: 'Please select the connected regions!',
                            type: 'array',
                        },
                        ]}
                    >
                        <Select mode="multiple" placeholder="Please select favourite colors">
                            {fields.map(f => f.key === field.key ? null : (
                                <Option value={f.key}>Region {f.key}</Option>
                            ))}
                        </Select>
                    </FormItem>
                </FormItem>
            </Panel>
        </Collapse>
    )
}

function Vaccination() {
    return(
        <>
            <FormItem 
                label="Strategy"
                name="vaccination_strategy"
            >
                <Radio.Group>
                    <Radio.Button value="none">None</Radio.Button>
                    <Radio.Button value="random">Random</Radio.Button>
                </Radio.Group>
            </FormItem>
            <FormItem
                label="Start time"
                name="T_v"
            >
                <Input />
            </FormItem>
            <FormItem
                label="Vaccines available per time step"
                labelCol={{ span: 24 }}
                name="n_v"
                style={{ display: 'flex', flexDirection: 'column', marginLeft: '11px'}}
            >
                <Input style={{width: '256px'}}/>
            </FormItem>
        </>
    )
}

function AgentGroup({fields, field, remove}) {
    const formatter = (value) => {
        return `${value}%`;
    }
    const marks = {
        0: '0',
        20: '20',
        40: '40',
        60: '60',
        80: '80',
        100: '100',
    }

    return(
        <Collapse>
            <Panel header={`Group ${field.key}`} key={field.key}                 extra={
                    fields.length > 1 ? (
                    <MinusCircleOutlined
                    className="dynamic-delete-button"
                    onClick={() => remove(field.name)} 
                    />) : null
                    }> 
                <FormItem
                    {...formItemLayout}
                    // {...(index === 0 ? formItemLayout : formItemLayoutWithOutLabel)}
                    // label={index === 0 ? 'Label' : ''}
                    required={false}
                    key={field.key}
                    noStyle
                >
                    <FormItem
                        {...field}
                        label="Infection Attempts"
                        labelCol={{span: 14}}
                        wrapperCol={{span:20}}                        
                        name={[field.name, 'n_i']}
                        fieldKey={[field.fieldKey, 'n_i']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="" style={{width: '80%'}} />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Infection Attempts (Asymptomatic)"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:20}}                        
                        name={[field.name, 'n_ai']}
                        fieldKey={[field.fieldKey, 'n_ai']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="" style={{width: '80%'}} />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Susceptibility"
                        labelCol={{span: 10}}
                        wrapperCol={{span:24}}                        
                        name={[field.name, 'susceptibility']}
                        fieldKey={[field.fieldKey, 'susceptibility']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="" style={{width: '80%'}} />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Infectiousness"
                        labelCol={{span: 11}}
                        wrapperCol={{span:26}}                        
                        name={[field.name, 'p_i']}
                        fieldKey={[field.fieldKey, 'p_i']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Slider
                        tipFormatter={formatter}
                            marks={marks}
                            />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Infectiousness Asymptomatic"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'p_ai']}
                        fieldKey={[field.fieldKey, 'p_ai']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Slider
                        tipFormatter={formatter}
                            marks={marks}
                            />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Travel Probability"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'p_t']}
                        fieldKey={[field.fieldKey, 'p_t']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Slider
                        tipFormatter={formatter}
                            marks={marks}
                            />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Travel Probability (Asymptomatic)"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'p_at']}
                        fieldKey={[field.fieldKey, 'p_at']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Slider
                        tipFormatter={formatter}
                            marks={marks}
                            />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Vaccine Effectiveness"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'p_v']}
                        fieldKey={[field.fieldKey, 'p_v']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Slider
                        tipFormatter={formatter}
                            marks={marks}
                            />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Vaccine Duration"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'd_v']}
                        fieldKey={[field.fieldKey, 'd_v']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="" style={{width: '80%'}} />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Infection Duration"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'd_i']}
                        fieldKey={[field.fieldKey, 'd_i']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="" style={{width: '80%'}} />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Infection Duration (Asymptomatic)"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'd_ai']}
                        fieldKey={[field.fieldKey, 'd_ai']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                        <Input placeholder="" style={{width: '80%'}} />
                    </FormItem>
                    <FormItem
                        {...field}
                        label="Probability of becoming asymptomatic"
                        labelCol={{span: 24}}
                        // wrapperCol={{span:26}}                        
                        name={[field.name, 'a_p']}
                        fieldKey={[field.fieldKey, 'a_p']}
                        validateTrigger={['onChange', 'onBlur']}
                        // noStyle
                    >
                         <Slider   
                            tipFormatter={formatter}
                                marks={marks}
                        />
                    </FormItem>
                </FormItem>
            </Panel>
        </Collapse>
    )
}



function SimulationForm() {
    const [vaccineOnOff, setVaccine] = useState(false);

    const onFinish = (values) => {
        console.log("Success:", values);
    };

    const onFinishFailed = (errorInfo) => {
        console.log("Failed:", errorInfo);
    };
    
    const onVaccinationSwitch = (checked) => {
        console.log("vaccine was set to: ", vaccineOnOff)
        console.log(`switch to ${checked}`);
        setVaccine(checked);
        console.log("vaccine is  set to: ", vaccineOnOff)
    }

    return(
        <>
            <Form
                {...layout}
                name="basic"
                initialValues={{
                    T: 100,
                    regions: [{
                        graph_type: "nw_small_world",
                        population: "12",
                        connections: []
                    }],
                    agent_groups: [
                        {
                            a_p: 30,
                            d_ai: "14",
                            d_i: "13",
                            d_v: "12",
                            n_ai: "20",
                            n_i: "10",
                            p_ai: 41,
                            p_at: 50,
                            p_i: 22,
                            p_t: 38,
                            p_v: 24
                        }
                    ]

                }}
                onFinish={onFinish}
                onFinishFailed={onFinishFailed}
            > 
                <Collapse accordion ghost expandIconPosition="right">
                    <Panel header="General Settings" key="gen-set">
                        <FormItem 
                            label="Timesteps"
                            name="T"
                        >
                            <Input />
                        </FormItem>
                        <FormItem 
                            label="Initial Infections"
                            labelCol= {{span: 11}}
                            name="initial_infections"
                        >
                            <Input />
                        </FormItem>
                        <FormItem 
                            label="Vaccination"
                            name="isVaccinating"
                        >
                            <Switch onChange={onVaccinationSwitch}/>
                        </FormItem>
                        {
                            vaccineOnOff && <Vaccination />
                        }
                    </Panel>
                    <Panel header="Regions" key="regions-set"> 
                        <FormList
                            name="regions"
                        >
                            {(fields, {add, remove}, { errors }) => (
                                <>
                                    <Collapse accordion>

                                    {fields.map((field, index) => (
                                        <Region fields={fields} field={field} remove={remove}/>
                                    ))}
                                    </Collapse>
                                    <FormItem>
                                        <Button
                                            type="dashed"
                                            onClick={() => add()}
                                            style={{width: '100%'}}
                                            icon={<PlusOutlined />}
                                        >
                                            Add Region
                                        </Button>
                                    </FormItem>
                                </>
                            )}    
                        </FormList>    

                    </Panel>
                    <Panel header="Agent Groups" key="groups-set">    
                        <FormList
                            name="agent_groups"
                        >
                            {(fields, {add, remove}, { errors }) => (
                                <>
                                    <Collapse accordion>
                                    {fields.map((field, index) => (
                                        <AgentGroup fields={fields} field={field} remove={remove}/>
                                    ))}
                                    </Collapse>
                                    <FormItem>
                                        <Button
                                            type="dashed"
                                            onClick={() => add()}
                                            style={{width: '100%'}}
                                            icon={<PlusOutlined />}
                                        >
                                            Add Group
                                        </Button>
                                    </FormItem>
                                </>
                            )}    
                        </FormList>  
                    </Panel>
                </Collapse>
                 
                <FormItem {...tailLayout}>
                    <Button type="primary" htmlType="submit">
                        Submit
                    </Button>
                </FormItem>   
            </Form>

        </>
    );
}

export default SimulationForm;
