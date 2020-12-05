# Imperative and Object-Oriented Programming Methodology
# Uppsala University - 2020 Autumn
# Copyright (c) 2020 group CovSim2: Henrik Schulze,
# Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
COMPILER=g++ -c
COMPILE_FLAGS= -Wall -g -fopenmp -o
LINKER=g++
LINK_FLAGS= -fopenmp -lstdc++fs -o
DEPENDEES = obj/Agent.o obj/Graph.o obj/Results.o obj/Simulator.o

obj/%.o: src/%.cpp
	$(COMPILER) $? $(COMPILE_FLAGS) $@

bin/%: obj/%.o
	$(LINKER) $? $(LINK_FLAGS) $@

# Run any program binary:
%: bin/%
	time $?

# Run any program with valgrind:
%_memtest: bin/%
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes $?

bin/matrix_test: obj/matrix_test.o $(DEPENDEES)
	$(LINKER) $? $(LINK_FLAGS) $@

bin/cov-sim: obj/cov-sim.o $(DEPENDEES)
	$(LINKER) $? $(LINK_FLAGS) $@

clean: clean_binaries clean_objects

clean_objects:
	rm -f obj/*.o
	mkdir -p bin obj

clean_binaries:
	rm -f bin/*
	mkdir -p bin obj

.PHONY: clean_binaries clean_objects clean

.PRECIOUS: bin/trivial bin/matrixC19
