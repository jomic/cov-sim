# Project in Computer Systems
# Uppsala University - 2020 Autumn
# Copyright (c) 2020 group CovSim2: Henrik Schulze,
# Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
COMPILER=g++ -c
COMPILE_FLAGS= -Wall -g -fopenmp -o
LINKER=g++
LINK_FLAGS= -fopenmp -lstdc++fs -o
DEPENDEES = obj/Agent.o obj/Graph.o obj/Results.o obj/Simulator.o obj/IOStreamHandler.o obj/GraphGeneration.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(COMPILER) $? $(COMPILE_FLAGS) $@

bin/%: obj/%.o $(DEPENDEES)
	mkdir -p $(@D)
	$(LINKER) $? $(LINK_FLAGS) $@

# Run any program binary:
%: bin/%
	time $?
	@echo ${OS}

# Run any program with valgrind:
%_memtest: bin/%
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes $?

clean: clean_binaries clean_objects

clean_objects:
	rm -f obj/*.o

clean_binaries:
	rm -f bin/*

.PHONY: clean_binaries clean_objects clean

.PRECIOUS: $(DEPENDEES) bin/trivial bin/cov-sim bin/matrix_test bin/matrixC19
