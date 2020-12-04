#include "Simulator.hpp"
#include "Results.hpp"
#include "Graph.hpp"

void Simulator::infect_initial(Graph& edges, int n) {
  // TODO: Right now just takes the first n
  for (int i = 0; i < (int)edges.node_values.size() && i < n; i++) {
    edges.node_values[i].infect(0);
  }
}

void Simulator::iterate(Results& result, Graph& edges, int t) {
  // // // int days_sick = settings.days_sick;
  for (Agent &node : edges.node_values) {
    node.update_results(t, result);
    if (node.is_infected(t)) {
      node.try_infecting_neighbours(t, edges);
      node.update_infection(t, this->days_sick);
    }
  }
}

Results Simulator::simulate(Graph& edges) {
  Results result;
  infect_initial(edges, this->initial_infections);
  for (int t = 1; t <= this->t_end; t++) {
    result.prepare_new_result();
    iterate(result, edges, t);
    // // // iterate(result, edges, settings, t);
  }
  return result;
}

// // // Results Simulator::simulate(settings_t& settings, Graph& edges) {
  // // // Results result;
  // // // infect_initial(edges, settings.initial_infections);
  // // // for (int t = 1; t <= settings.t_end; t++) {
    // // // result.prepare_new_result();
    // // // iterate(result, edges, settings, t);
  // // // }
  // // // return result;
// // // }
