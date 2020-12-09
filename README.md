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

and like-wise for `matrix_test` and `matrixC19`. If you want to leave **all the other** executable binaries intact, but want to remove (and recreate) all the object files, then do:

- `tools/cleanCnR <main-file>`

where you should replace `<main-file>` by any of the main files.

This is work in progress!

---

Additional notes will be written here ...