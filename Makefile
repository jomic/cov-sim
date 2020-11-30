# Imperative and Object-Oriented Programming Methodology
# Uppsala University - 2020 Autumn
# Copyright (c) 2020 group CovSim2: Henrik Schulze,
# Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
COMPILER=g++ -c
CFLAGS= -Wall -g -fopenmp -o
LINKER=g++
LFLAGS= -fopenmp -o

obj/%.o: src/%.cpp
	$(COMPILER) $(CFLAGS) $@ $?

bin/%: obj/%.o
	$(LINKER) $(LFLAGS) $@ $?

# Run any program binary:
%: bin/%
	time $?

# Run any program with valgrind:
%_memtest: bin/%
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes $?

targets: bin/trivial

clean:
	rm -fr bin/ obj/
	mkdir -p bin
	mkdir -p obj

.PHONY: clean targets
