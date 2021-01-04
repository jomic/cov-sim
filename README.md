# cov-sim
In this project a Covid-19 simulator using a Microscopic Agent-Based Model has been built.

There is [more extensive documentation](docs/Docs.md).

For the time being, there are **four** source files containing a `main` function, namely `cov-sim.cpp`, `matrixDemo.cpp`, `matrixC19.cpp` and `gen-graph.cpp`.

The `gen-graph.cpp` file is used only for generating graphs and does not run any simulation.

Compile and run with *default values* by doing either of:

( If you get `Permission denied` try first running `chmod u+x tools/*`. )

- `tools/compileAndRun cov-sim`
- `tools/compileAndRun matrixDemo`
- `tools/compileAndRun matrixC19`
- `tools/compileAndRun gen-graph`

Once an executable binary file runs without errors and you don't want to make any more changes to the source code, simply run it as: `bin/cov-sim,` `bin/matrixDemo,` `bin/matrixC19` or `bin/gen-graph` with or without arguments.

If you want to make a clean compilation (removing **all** binary executables and **all** object files), then do:

- `make clean && make <main-file>`

where you should replace `<main-file>` by `cov-sim`, `matrixDemo` ,`matrixC19` or `gen-graph`.
If you want to leave **all the other** executable binaries intact, but want to remove (and recreate) all the object files, then do:

- `tools/cleanCnR <main-file>`

---

# Settings

The `cov-sim` executable can be run using a JSON-formatted settings stream for inputting parameter values. It can also output the results of the simulation in JSON format. There are a couple of option flags available when running it:

- `-i` Use JSON-formatted settings from the standard input stream - typically from a file.
            (If the `-i` flag is not used, the parameters are set to their default values).
- `-o` Output JSON-formatted results to the standard output stream.
- `-t` If the `-o` flag is active, results are outputted in real-time as they become available.
            (If the `-t` flag is not used with `-o`, the results are all outputted when the simulation ends as a single JSON object).
- `-p` Plot the results in the terminal - along with the SAIVR values.
- `-s` Use a non-deterministic seed for the simulation.

For example, to run the program with settings from `matrix,L=100,D0=5.json`, a random seed, and output in `resultsMatrix.json`, you can run:

`bin/cov-sim -iops < matrix,L=100,D0=5.json > resultsMatrix.json`

Or, to use `matrix,L=100,D0=5.json`, a **non**-random seed, and make a plot in the terminal, run:

`bin/cov-sim -ip < matrix,L=100,D0=5.json`

The settings file is structured as follows.
Note that any value can be omitted, in which case it will be set to a default value.

```
{
    "select_all": /* Whether ALL neighbours can get infected each step */,
    "initial_infections": /* The number of initially infected agents */,
    "T":          /* The number of simulation time steps */,
    "T_v":        /* The time step at which vaccination starts */,
    "n_v":        /* The number of vaccinations available at each time step */,
    "vaccination_strategy": {},    /* See more below */
    "groups": [],                  /* See more below */
    "graph": {} or [],             /* See more below */
    "region_connections": [] or "" /* See more below */
}
```

The vaccination strategy used in the simulation is specified as an object with the mandatory field `"type"`. Currently, the available strategies available are:
- `"nothing"`: do not distribute vaccines at all.
- `"random"`: distribute the vaccines randomly.
- `"highest_density"`: prioritize agents with the most infected neighbours.
- `"lowest_density"`: prioritize agents with the least infected neighbours.
For example, the value of `"vaccination_strategy"` could be set as follows.

```
{
    "type": "random"
}
```

The settings for the groups is an array of objects of group parameters. These groups will be distributed randomly among agents when initializing the graph.

```
{
    "n_i":  /* How many infection attempts if infected */,
    "n_ai": /* How many infection attempts if asymptomatic */,
    "susceptibility":    /* How susceptible an agent is */,
    "p_i":  /* How infectious an agent is if infected */,
    "p_ai": /* How infectious an agent is if asymptomatic */,
    "p_t":  /* How probable is the agent to travel if infected */,
    "p_at": /* How probable is the agent to travel if asymptotic */,
    "p_v":  /* How probable a vaccine is to work */,
    "d_v":  /* How many time steps (_d_ays) until a vaccine works */,
    "d_i":  /* How many time steps until an infected agent is removed */,
    "d_ai": /* How many time steps until an asymptomatic agent is removed */,
    "a_p":  /* How probable an agent is to become asymptomatic if infected */
}
```

The graph setting is a JSON object that depends on the type of graph that should be generated. The `"type"` field is used to decide which type of graph is created. This field can also be set as an array of objects, in which case each object in the array will be used to create a graph for a different region.

```
{
    "type": "matrix", // A graph representation of a matrix
    "size":           /* The length of the side of the matrix */,
    "distance":       /* Measures the size of the neighbour sub-matrix */
}
```

```
{
    "type": "nw_small_world", // A Newman-Watts small world
    "N": /* Number of agents */,
    "k": /* Immediate neighbour range */,
    "p": /* Shortcut probability */
}
```

```
{
    "type": "file_format_simple", // A graph based on a file, simple file format
    "file_name": /* The name of the file being used */
}
```

```
{
    "type": "file_format_advanced", // Reading the graph from a file
    "file_name": /* The name of the file being used */
}
```

The regions can be connected to other regions, which makes it possible for agents to try to infect random agents in a neighbouring region. There are two ways of specifying the connections in the `"region_connections"` field. The first is an array of arrays of integers, wherein the numbers in the n:th array specify the indices of the regions that region n is connected to. The other is to specify a filename as a string, in which line n lists space-separated indices for regions connected to region n.

---

# Plotting

To plot the json output there is a python script using matplotlib, meaning that both python and matplotlib need to be installed (`python -m pip install matplotlib`).
An example of using this script from a file:

```
py/plot_sir.py < output.json
```
It is also possible to pipe the result from `cov-sim` directly. Note that the script is used for plotting the total results, rather than the real-time results. Therefore, the `-t` flag must be inactive:
```
bin/cov-sim -io < settings.json | py/plot_sir.py
```
Make sure to make the script runnable with `chmod +x py/plot_sir.py`.

---

# Graph generator

The `gen-graph` executable can be used to pre-generate Newman-Watts small world graphs, as this can be time-consuming. It has the following flags available:

- `-o` The graph should be put in the standard output stream, in a format readable by the `"file_format_advanced"` option of the settings file.
- `-r` The input to the program is an already-existing graph, and the graph generated should be added as a region.
- `-s` Use a randomized seed for the simulation.
- `-N` The number of agents.
- `-k` The number of connected immediate neighbours on each side of an agent.
- `-p` The probability of a given non-neighbourhood edge being created.

For example, to generate a *Newman-Watts small world* graph with ***two regions***
and store it in `sw-2R-fromFile.txt`, you could paste or type:

`bin/gen-graph -o -N 10000 -k 10 -p 0.001 | bin/gen-graph -or -N 5000 -k 15 -p 0.003 > sw-2R-fromFile.txt`

Note the `-r` flag in the command *to the right* of the pipe (`|`) character, which adds the graph generated *to the left* the pipe (`|`) as another region. When the `-r` flag  is used, the input to the executable (on the right side) should be a stream with already-existing graph data. This is the case in this example, since the `-o` flag is active in the command to the left of the pipe. The graph with 10 000 people is thus added as input when generating the graph with 5000 people.

------

# How to use a generated graph

If a graph has been saved to a file (under the format `"file_format_advanced"`), it can be read and used for a simulation. To use the graph stored in `sw-2R-fromFile.txt`, paste or type:

`bin/cov-sim -ip < sw-2R-fromFile.json`

where the file `sw-2R-fromFile.json` has the following content:

    {
        "select_all": false,
        "initial_infections": 1,
        "T": 120,
        "T_v": 10,
        "n_v": 20,
        "groups": [{
            "n_i": 1,
            "n_ai": 3,
            "susceptibility": 1.0,
            "p_i": 0.25,
            "p_ai": 0.35,
            "p_t": 0.001,
            "p_at": 0.001,
            "p_v": 0.9,
            "d_v": 28,
            "d_i": 14,
            "d_ai": 10,
            "a_p": 0.1
        }],
        "graph": [{
            "type": "file_format_advanced",
            "file_name": "sw-2R-fromFile.txt"
        }],
        "region_connections": [[1],[0]]
    }

------

# Simulation examples - graph *not* from a file

If a graph is small enough, you need not generate it first and store it to a file.
For example, the following generates a graph and runs a simulation with the same parameters as above:

`bin/cov-sim -ip < sw-2Regions.json`

Another example generates a graph and runs a simulation for _**three**_ regions:

`bin/cov-sim -ip < sw-3Regions.json`

------
