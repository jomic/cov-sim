const server = require('mockttp').getLocal();
const mx_data = require("../src/data/mx_example.json");
const nw_data = require("../src/data/nw_example.json");
const test100 = require("../src/data/test_output_gw_sw.json");
const test100p01 = require("../src/data/test_output_sw100p01.json");



function randomState() {
    return Math.floor(Math.random()*4);
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

async function init(port) {
    console.log("starting server");
    await server.start(port);
    console.log("server started");
    // mockServer.stop();
    // console.log("server stopped");

    server.post("/api/simulation").thenCallback(request => {
        console.log("post: ", request);
        const graph = genRandomTree(1000);
        // something 
        return {
            statusCode: 200,
            json: {
                graph: test100p01, 
                // {
                //     "nodes": [
                //         {"id": 1, "state": 1},
                //         {"id": 2, "state": 2},
                //         {"id": 3, "state": 3},
                //         {"id": 4, "state": 4},
                //         {"id": 5, "state": 0},
                //         {"id": 6, "state": 0},
                //         {"id": 7, "state": 1},
                //         {"id": 8, "state": 1},
                //         {"id": 9, "state": 2},
                //         {"id": 10, "state": 4}
                
                //     ],
                //     "links": [
                //         {"source": 1, "target": 2, "value": 1},
                //         {"source": 1, "target": 3, "value": 1},
                //         {"source": 2, "target": 3, "value": 1},
                //         {"source": 2, "target": 4, "value": 1},
                //         {"source": 2, "target": 5, "value": 1},
                //         {"source": 2, "target": 7, "value": 1},
                //         {"source": 5, "target": 6, "value": 1},
                //         {"source": 6, "target": 7, "value": 1},
                //         {"source": 6, "target": 8, "value": 1},
                //         {"source": 4, "target": 9, "value": 1},
                //         {"source": 5, "target": 10, "value": 1}
                
                //     ]
                // },
                plot: {
                    s: 100,
                    a: 10,
                    i: 2,
                    v: 0,
                    r: 0,
                }
            } 
        };
    })

    server.put("/api/simulation").thenCallback(request => {
        console.log("post: ", request);
        const graph = genRandomTree(200);
        console.log("random graph: ", graph);
        // something 
        return {
            statusCode: 200,
            json: {
                graph: test100p01,
                // {
                //     "nodes": [
                //         {"id": 1, "state": randomState()},
                //         {"id": 2, "state": randomState()},
                //         {"id": 3, "state": randomState()},
                //         {"id": 4, "state": randomState()},
                //         {"id": 5, "state": randomState()},
                //         {"id": 6, "state": randomState()},
                //         {"id": 7, "state": randomState()},
                //         {"id": 8, "state": randomState()},
                //         {"id": 9, "state": randomState()},
                //         {"id": 10, "state": randomState()},                
                //     ],
                //     "links": [
                //         {"source": 1, "target": 2, "value": 1},
                //         {"source": 1, "target": 3, "value": 1},
                //         {"source": 2, "target": 3, "value": 1},
                //         {"source": 2, "target": 4, "value": 1},
                //         {"source": 2, "target": 5, "value": 1},
                //         {"source": 2, "target": 7, "value": 1},
                //         {"source": 5, "target": 6, "value": 1},
                //         {"source": 6, "target": 7, "value": 1},
                //         {"source": 6, "target": 8, "value": 1},
                //         {"source": 4, "target": 9, "value": 1},
                //         {"source": 5, "target": 10, "value": 1},
                //         {"source": 5, "target": 10, "value": 1},
                //     ]
                // },
                plot: {
                    s: Math.round(Math.random()*20 + 90),
                    a: Math.round(Math.random()*10 + 5),
                    i: Math.round(Math.random()*10 + 1),
                    v: Math.round(Math.random()*10 + 10),
                    r: Math.round(Math.random()*10 + 20),
                }
            } 
        };
    })

    server.get("/api/simulation").thenCallback(request => {
        console.log("request test");
        return {
            statusCode: 200,
            json: {
                hello: "world"
            } 
        };
    });


}




init(8080);




