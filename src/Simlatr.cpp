#include "Graph.hpp"
#include "Results.hpp"
#include "Simlatr.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include "VcStrgies.hpp"
#include "Utils.hpp"

Simlatr::Simlatr(std::shared_ptr<VcStrgy>& vs) {
  vac_strat = vs;
}

void Simlatr::infect_initial(Graph& graf, int n) {
  std::set<int> rand_index = unique_random_numbers(n, graf.node_count());
  for (auto i : rand_index) {
    graf.agents[i].infect(0);
  }
}

void Simlatr::iterate(Results& results, Graph& graf, int t) {
  vac_strat->vaccinate(graf, t);
  int current_id = 0;
  int current_region = -1;
  for (Agent &node : graf.agents) {
    if (graf.get_agent_region(current_id++) != current_region) {
      current_region++;
      results.prepare_new_region();
    }
    node.update_results(t, results);
    if (node.is_infected(t)) {
      if (node.is_travelling(t)) {
	node.try_infecting_on_travel(t, graf);
      }
      else {
	if (select_all)
	  node.try_infecting_neighbours(t, graf);
	else
	  node.try_infecting_n_neighbours(t, graf);	
      }
      node.update_infection(t);
    }
    else if (node.is_vaccinated_susceptible(t)) {
      node.update_vaccination(t);
    }
  }
}

Results Simlatr::simulate(Graph& graf, bool print_each_result) {
  Results results;
  
  /**
    Just for now: settings are based on document names,
    so the variables used in the simulator are assigned 
    these values here until we decide which names to use.
  */
  initial_infections = N_init_infected;
  t_end = T;
  
  
  infect_initial(graf, initial_infections);
  for (int t = 1; t <= t_end; t++) {
    results.prepare_new_result();
    iterate(results, graf, t);
    if (print_each_result)
      results.write_last_to_output_stream(std::cout, true);
  }
  return results;
}

Results Simlatr::simulate(Graph& graf) {
  return simulate(graf, false);
}
