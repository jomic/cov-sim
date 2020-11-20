#include "Simulator.hpp"
#include "Result.hpp"
#include "Grid.hpp"

void Simulator::iterate(Result& result, Grid& edges, int t) {
  for (int i = 0; i < edges.node_values.size(); i++) {
    Individual node = edges.node_values[i];
    node.update_result(result);
    if (node.is_infected()) {
      node.try_infecting_neighbours(t, edges);
      node.update_infection(t, 14); // TODO: get duration in better way
    }
  }
}

Result Simulator::simulate(settings_t& settings, Grid& edges) {
  Result result;
  for (int t = 0; t < settings.t_end; t++) {
    result.prepare_new_result();
    iterate(result, edges, t);
  }
  return result;
}
