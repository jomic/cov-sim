// data for the plot

const susceptible = [
    3910, 3706, 2850, 4005, 3750, 2912, 3200, 3645, 4205, 3211, 3354, 3904
];

const infected = [
    3910, 3706, 2850, 4005, 3750, 2912, 3200, 3645, 4205, 3211, 3354, 3904
];

// etc.

// labels for the plot
export const yearLabels = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];

// json for the graph

const jsonGraph = {
    "nodes": [
        {"id": 1, "state": 1},
        {"id": 2, "state": 2},
        {"id": 3, "state": 3},
        {"id": 4, "state": 4},
        {"id": 5, "state": 0},
        {"id": 6, "state": 0},
        {"id": 7, "state": 0}
    ],
    "links": [
        {"source": 1, "target": 2, "value": 1},
        {"source": 1, "target": 3, "value": 10},
        {"source": 2, "target": 3, "value": 1},
        {"source": 2, "target": 4, "value": 1},
        {"source": 2, "target": 5, "value": 1},
        {"source": 2, "target": 7, "value": 1},
        {"source": 5, "target": 6, "value": 1},
        {"source": 6, "target": 7, "value": 1}
    ]
}

// json for the matrix
const jsonMatrix = {
    "nodes": [
        {"x": 1, "y": 1, "state": 1},
    ]
}

