#include "Simulator.hpp"
#include "Results.hpp"
#include "Graph.hpp"

void Simulator::infect_initial(Graph& edges, int n) {
  // TODO: Right now just takes the first n
  for (int i = 0; i < edges.node_values.size() && i < n; i++) {
    edges.node_values[i].infect(0);
  }
}

void Simulator::iterate(Results& result, Graph& edges, settings_t& settings, int t) {
  int d = settings.d;
  for (Individual &node : edges.node_values) {
    node.update_results(t, result);
    if (node.is_infected(t)) {
      node.try_infecting_neighbours(t, edges);
      node.update_infection(t, d);
    }
  }
}

Results Simulator::simulate(settings_t& settings, Graph& edges) {
  Results result;
  infect_initial(edges, settings.initial_infections);
  for (int t = 1; t <= settings.t_end; t++) {
    result.prepare_new_result();
    iterate(result, edges, settings, t);
  }
  return result;
}
