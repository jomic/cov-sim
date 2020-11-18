#include "Graph.hpp"

class Individual {
public:
	Individual(int id, Graph edges); // TODO: I assume Graph should be a reference instead?

	/*
		Check if the individual can infect people at time t.
	*/
	bool is_infected(int t);

	/*
		Check if the individual can still be infected at time t.
	*/
	bool is_susceptible(int t);

	/*
		Infect the individual at time t.
	*/
	void infect(int t);

	/*
		Make the individual try to infect all of its neighbours at time t.
	*/
	void try_infecting_neighbours(int t);

	/*
		Update the individuals infection, given that an infection last for d time steps.
	*/
	void update_infection(int t, int d);
};