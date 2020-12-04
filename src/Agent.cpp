#include <iterator>
#include <vector>
#include "Graph.hpp"
#include "Agent.hpp"
#include "Results.hpp"

void Agent::try_infecting_neighbour(int t, int target_id, Graph& edges) {
  Agent& n = edges.node_values[target_id];
  if (n.is_susceptible(t)) {
    float roll = (float)rand() / (float)RAND_MAX;
    if (roll < n.susceptibility)
      n.infect(t);
  }
}

Agent::Agent(int id, float susceptibility)
  : id(id), susceptibility(susceptibility) {
  s = true;
  i = false;
  r = false;
  infected_on = -1;
}

Agent::Agent(int id)
  : Agent::Agent(id, 0.1f) {}

bool Agent::is_infected(int t) {
  return i && t > infected_on;
}

bool Agent::is_susceptible(int t) {
  return s;
}

void Agent::infect(int t) {
  if (s) {
    s = false;
    i = true;
    infected_on = t;
  }
}

void Agent::try_infecting_neighbours(int t, Graph& edges) {
  if (!is_infected(t))
    return;
  std::vector<int> n_ids = edges.neighbours(id);
  for (auto n_id : n_ids)
    try_infecting_neighbour(t, n_id, edges);
}

void Agent::update_infection(int t, int days_sick) {
  if (i && infected_on + days_sick < t) {
    i = false;
    r = true;
  }
}

void Agent::update_results(int t, Results& results) {
  if (s || (i && t == infected_on))
    results.add_susceptible();
  else if (i)
    results.add_infected();
  else if (r)
    results.add_removed();
}
