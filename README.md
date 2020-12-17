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

This is work in progress!

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
	"graph": {} /* See more below */
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
	"p_v": /* How probable a vaccine is to work */,
	"d_v": /* How long it takes for a vaccine to work */,
	"d_i": /* How many time steps until an infected agent is removed */,
	"d_ai": /* How many time steps until an asymptomatic agent is removed */,
	"a_p": /* How probable an agent is to become asymptomatic if infected */
}
```

The graph setting is a json object that depends on the type of graph that should be generated. The `"type"` field is used to decide which type of graph is created.

```
{
	"type": "matrix", // A graph representation of a matrix
	"size": /* The length of the side of the matrix */,
	"distance": /* The range of each agent in the matrix */
}
```

```
{
	"type": "file", // A graph based on a file
	"file_name": /* The name of the file being used */
}
```

---

# Plotting

To plot the json output there is a python script included using matplotlib, that means that both python and matplotlib needs to be installed (`python -m pip install matplotlib`). An example of using this script from a file:
```
py/plot_sir.py < output.json
```
It is also possible to pipe the result from `cov-sim` directly:
```
../src/cov-sim -i -o < settings.json | ./plot_sir.py
```
Make sure to make the script runnable with `chmod +x py/plot_sir.py`.

---
