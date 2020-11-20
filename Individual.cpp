#include <vector>
#include <iterator>
#include "Graph.hpp"
#include "Individual.hpp"
#include "Results.hpp"
#include <iostream>


void Individual::try_infecting_neighbour(int t, int target_id, Graph& edges) {
  Individual& n = edges.node_values[target_id];
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
  susceptibility = 0.1f;
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
  std::vector<int> ns = edges.neighbours(id);
  for (int i = 0; i < ns.size(); i++) {
    try_infecting_neighbour(t, ns[i], edges);
  }
}

void Individual::update_infection(int t, int d) {
  if (i && infected_on + d < t) {
    i = false;
    r = true;
  }
}

void Individual::update_results(int t, Results& results) {
  if (s || (i && t == infected_on))
    results.add_susceptible();
  else if (i)
    results.add_infected();
  else if (r)
    results.add_removed();
}
