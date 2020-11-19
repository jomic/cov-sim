#include <vector>
#include <iterator>
#include "Graph.hpp"
#include "Individual.hpp"

// TODO: Might be a better way to randomize
Individual::void try_infecting_neighbour(int t, int id, Graph& edges) {
  Individual n = edges.get_node(id);
  if (n.is_susceptible(t)) {
    float roll = (float)rand() / (float)RAND_MAX;
    if (roll < n.susceptibility)
      n.infect(t);
  }
}
Individual::Individual(int id) {
  this->id = id;
  s = true;
  i = false;
  r = false;
  susceptibility = 0.01f;
  infected_on = -1;
}

bool Individual::is_infected(int t) {
  return i && t > infected_on;
}

bool Individual::is_susceptible(int t) {
  return s;
}

void Individual::infect(int t) {
  if (s) {
    s = false;
    i = true;
    infected_on = t;
  }
}

void Individual::try_infecting_neighbours(int t, Graph& edges) {
  if (!is_infected(t))
    return;
  std::vector<int> ns = edges.get_neighbours(id);
  for (std::vector<int>::iterator it = ns.begin(); it != ns.end(); it++) {
    try_infecting_neighbour(t, *it, edges);
  }
}

void Individual::update_infection(int t, int d) {
  if (i && d < t - infected_on) {
    i = false;
    r = true;
  }
}
