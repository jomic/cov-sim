# cov-sim
Covid simulator using Microscopic Agent-Based Model.

For the time being, there are **two** source files containing a `main` function, namely `cov-sim.cpp` and `matrix_test.cpp`.

Compile and run with default values by doing either of:

- `tools/compileAndRun cov-sim`
- `tools/compileAndRun matrix_test`

Once an executable binary file runs without errors and you don't want to make any more changes to the source code, simply run it as: `bin/cov-sim` or `bin/matrix_test` with or withour parameters.

This is work in progress! :-)

---

[ 

- `make cov-sim; make cov-sim;`
- `make matrix_test; make matrix_test;`

OR simply run the `make` command twice. Why it must to be run **twice** is not quite clear, but for now we simply accept running `make` twice as a workaround (https://stackoverflow.com/questions/26770875).

Changes to any `.hpp` file requires the corresponding executable binary to be removed before running `make`. For example, after a change in `Simulator.hpp`, do one of: ]