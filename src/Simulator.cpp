#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include <vector>
#include <iostream>
#include "VaccinationStrategies.hpp"
#include "Utilities.hpp"



Simulator::Simulator(VaccinationStrategy& vs) {
  vac_strat = &vs;
}

void Simulator::infect_initial(Graph& edges, int n) {
  std::set<int> rand_index = unique_random_numbers(n, edges.node_count());
  for (auto i : rand_index) {
    edges.node_values[i].infect(0);
  }
}

void Simulator::iterate(Results& results, Graph& edges, int t) {

  vac_strat->vaccinate(edges, t);
  
  for (Agent &node : edges.node_values) {
    node.update_results(t, results);
    if (node.is_infected(t)) {
      if (select_all)
	node.try_infecting_neighbours(t, edges);
      else
	node.try_infecting_n_neighbours(t, edges);
      node.update_infection(t);
    }
    else if (node.is_vaccinated_susceptible(t)) {
      node.update_vaccination(t);
    }
  }
}

Results Simulator::simulate(Graph& edges) {
  Results results;
  
  /*
    Just for now: settings are based on document names,
    so the variables used in the simulator are assigned 
    these values here until we decide which names to use.
  */
  initial_infections = N;
  t_end = T;
  
  
  infect_initial(edges, initial_infections);
  for (int t = 1; t <= t_end; t++) {
    results.prepare_new_result();
    iterate(results, edges, t);
  }
  return results;
}
