#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include "VaccinationStrategies.hpp"
#include "Utilities.hpp"

Simulator::Simulator(std::shared_ptr<VaccinationStrategy>& vs) {
  vac_strat = vs;
}

void Simulator::infect_initial(Graph& edges, int n) {
  std::set<int> rand_index = unique_random_numbers(n, edges.node_count());
  for (auto i : rand_index) {
    edges.node_values[i].infect(0);
  }
}

void Simulator::iterate(Results& results, Graph& edges, int t) {
  vac_strat->vaccinate(edges, t);
  int current_id = 0;
  int current_region = -1;
  for (Agent &node : edges.node_values) {
    if (edges.get_agent_region(current_id++) != current_region) {
      current_region++;
      results.prepare_new_region();
    }
    node.update_results(t, results);
    if (node.is_infected(t)) {
      if (node.is_travelling(t)) {
	node.try_infecting_on_travel(t, edges);
      }
      else {
	if (select_all)
	  node.try_infecting_neighbours(t, edges);
	else
	  node.try_infecting_n_neighbours(t, edges);	
      }
      node.update_infection(t);
    }
    else if (node.is_vaccinated_susceptible(t)) {
      node.update_vaccination(t);
    }
  }
}

Results Simulator::simulate(Graph& edges, bool print_each_result) {
  Results results;
  
  /**
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
    if (print_each_result)
      results.write_last_to_output_stream(std::cout, true);
  }
  return results;
}

Results Simulator::simulate(Graph& edges) {
  return simulate(edges, false);
}
