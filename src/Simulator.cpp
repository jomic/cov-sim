#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"

void Simulator::infect_initial(Graph& edges, int n) {
  // TODO: Right now just takes the first n
  for (int i = 0; i < (int)edges.node_values.size() && i < n; i++) {
    edges.node_values[i].infect(0);
  }
}

void Simulator::iterate(Results& results, Graph& edges, int t) {
  for (Agent &node : edges.node_values) {
    node.update_results(t, results);
    if (node.is_infected(t)) {
      node.try_infecting_neighbours(t, edges);
      node.update_infection(t, days_sick);
    }
  }
}

Results Simulator::simulate(Graph& edges) {
  Results results;
  infect_initial(edges, initial_infections);
  for (int t = 1; t <= t_end; t++) {
    results.prepare_new_result();
    iterate(results, edges, t);
  }
  return results;
}
