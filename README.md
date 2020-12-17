# cov-sim
Covid simulator using Microscopic Agent-Based Model.

For the time being, there are **three** source files containing a `main` function, namely `cov-sim.cpp`, `matrix_test.cpp` and `matrixC19.cpp`.

Compile and run with default values by doing either of:

- `tools/compileAndRun cov-sim`
- `tools/compileAndRun matrix_test`
- `tools/compileAndRun matrixC19`

Once an executable binary file runs without errors and you don't want to make any more changes to the source code, simply run it as: `bin/cov-sim,` `bin/matrix_test` or `bin/matrixC19` with or without arguments.

If you want to make a clean compilation (removing **all** binary executables and **all** object files), then do:

- `make clean && make cov-sim`

and like-wise for `matrix_test` and `matrixC19`.
If you want to leave **all the other** executable binaries intact, but want to remove (and recreate) all the object files, then do:

- `tools/cleanCnR <main-file>`

where you should replace `<main-file>` by any of the main files.

---

# Settings

The `cov-sim` exacutable can be run using a json-formatted settings stream as input. This executable can also output the results of the simulation as json. There are a couple of option flags available when running it:

- `-i` Use json-formatted settings from the standard input stream (otherwise a de- fault is used)
- `-o` Output json-formatted results to the standard output stream
- `-r` Use a non-deterministic seed for the simulation
- `-p` Print the results as a plot in the terminal

For example, to run the program with settings from `settings.json`, output in `results.json`, a random seed, and a plot in the terminal, you can run:

`bin/cov-sim -iorp < settings.json > output.json`

The settings file is structured as follows. Note that any value can be omitted, in which case it will be set to a default value.

```
{
	"select_all": /* Whether all neighbours get infection attempts each step or just some */,
	"N": /* The number of initial infected */,
    	"T": /* The number of simulation time steps */,
	"T_v": /* The time step at which vaccination starts */,
	"n_v": /* The number of vaccinations available at each time step */,
	"groups": [], /* See more below */
	"graph": {} or [], /* See more below */
	"region_connections": [] or "" /* See more below */
}
```

The settings for the groups is an array of objects of group parameters. These groups will be distributed randomly among agents when initializing the graph.

```
{
	"n_i": /* How many infection attempts if infected */,
	"n_ai": /* How many infection attempts if asymptomatic */,
	"s": /* How susceptible an agent is */,
	"p_i": /* How infectious an agent is if infected */,
	"p_ai": /* How infectious an agent is if asymptomatic */,
	"p_t": /* How probable is the agent to travel if infected */,
	"p_at": /* How probable is the agent to travel if asymptotic */,
	"p_v": /* How probable a vaccine is to work */,
	"d_v": /* How long it takes for a vaccine to work */,
	"d_i": /* How many time steps until an infected agent is removed */,
	"d_ai": /* How many time steps until an asymptomatic agent is removed */,
	"a_p": /* How probable an agent is to become asymptomatic if infected */
}
```

The graph setting is a json object that depends on the type of graph that should be generated. The `"type"` field is used to decide which type of graph is created. This field can also be set as an array of objects, in which case each object in the array will be used to create a graph for a different region.

```
{
	"type": "matrix", // A graph representation of a matrix
	"size": /* The length of the side of the matrix */,
	"distance": /* The range of each agent in the matrix */
}
```

```
{
	"type": "nw_small_world", // A Newman-Watts small world
	"l": /* Number of agents */,
	"k": /* Immediate neighbour range */,
	"p": /* Shortcut probability */
}
```

```
{
	"type": "file", // A graph based on a file
	"file_name": /* The name of the file being used */
}
```

```
{
	"type": "file_2", // An alternative graph based on a file
	"file_name": /* The name of the file being used */
}
```

The regions can be connected to other regions, which makes it possible for agents to try to infect random agents in a neighbouring region. There are two ways of specifying the connections in the `"region_connections"` field. The first is an array of arrays of integers, wherein the numbers in the n:th array specify the indices of the regions that region n is connected to. The other is to specify a filename as a string, in which line n lists spaced-seperated indices for regions connected to region n.

---

# Graph generator

The `graph_generator` executable can be used to pre-generate Newman-Watts small world graphs, as this can be time-consuming. It has the following flags available:

- `-o` The graph should be put in the standard output stream, in a format readable by the `"file_2"` option of the settings file.
- `-l` The number of agents.
- `-k` The number of connected immediate neighbours on each side of an agent.
- `-p` The probability of a given non-neighbourhood edge being created.
- `-r` The input to the program is an already-existing graph, and the graph generated should be added as a region.

For example, to generate a graph with two regions and store it in `graph.txt`, you could write:

`bin/graph_generator -o -l 10000 -k 10 -p 0.001 | bin/graph_generator -ro -l 5000 -k 15 -p 0.003 > graph.txt`

---

Additional notes will be written here ...