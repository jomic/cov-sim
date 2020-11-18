#include <vector>
#include <iterator>
#include "Graph.hpp"

class Individual {
private:
	int id;
	bool s;
	bool i;
	bool r;
	float susceptibility;
	int infected_on;
	Graph edges;


	// TODO: Might be a better way to randomize
	void try_infecting_neighbour(int t, int id) {
		Individual n = edges.get_node(id);
		if (n.is_susceptible(t)) {
			float roll = (float)rand() / (float)RAND_MAX;
			if (roll < n.susceptibility)
				n.infect(t);
		}
	}

public:
	Individual(int id, Graph edges) {
		this->id = id;
		this->edges = edges;
		s = true;
		i = false;
		r = false;
		susceptibility = 0.01f;
		infected_on = -1;
	}

	bool is_infected(int t) {
		return i && t > infected_on;
	}

	bool is_susceptible(int t) {
		return s;
	}

	void infect(int t) {
		if (s) {
			s = false;
			i = true;
			infected_on = t;
		}
	}

	void try_infecting_neighbours(int t) {
		if (!is_infected(t))
			return;
		std::vector<int> ns = edges.get_neighbours(id);
		for (std::vector<int>::iterator it = ns.begin(); it != ns.end(); it++) {
			try_infecting_neighbour(t, *it);
		}
	}

	void update_infection(int t, int d) {
		if (i && duration < t - infected_on) {
			i = false;
			r = true;
		}
	}
};