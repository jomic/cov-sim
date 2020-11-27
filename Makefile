CXX=g++
RM=rm -f
CPPFLAGS= -Wall -g

graphtest: matrix_test.cpp Graph.cpp Simulator.cpp Results.cpp Individual.cpp
	$(CXX) $(CPPFLAGS) -o graphtest matrix_test.cpp Graph.cpp Simulator.cpp Results.cpp Individual.cpp

clean:
	$(RM) #*#
	$(RM) *.o

plot: graphtest
	./graphtest
