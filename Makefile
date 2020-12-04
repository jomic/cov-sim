# Imperative and Object-Oriented Programming Methodology
# Uppsala University - 2020 Autumn
# Copyright (c) 2020 group CovSim2: Henrik Schulze,
# Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
COMPILER=g++ -c
COMPILE_FLAGS= -Wall -g -fopenmp -o
LINKER=g++
LINK_FLAGS= -fopenmp -lstdc++fs -o

obj/%.o: src/%.cpp
	$(COMPILER) $? $(COMPILE_FLAGS) $@

bin/%: obj/%.o obj/Agent.o obj/Graph.o obj/Results.o obj/Simulator.o
	$(LINKER) $? $(LINK_FLAGS) $@

# Run any program binary:
%: bin/%
	time $?

cov-sim: bin/cov-sim
	time $?

# Run any program with valgrind:
%_memtest: bin/%
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes $?

targets: bin/trivial

clean:
	rm -f bin/* obj/*.o
	mkdir -p bin obj

.PHONY: clean targets
