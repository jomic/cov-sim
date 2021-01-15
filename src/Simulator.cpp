#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include "VacStrats.hpp"
#include "Utils.hpp"

Simulator::Simulator(std::shared_ptr<VacStrat>& vs) {
  vac_strat = vs;
}

void Simulator::infect_initial(Graph& graf, int initially_infected) {
  std::set<int> rand_index
      = unique_random_numbers(initially_infected, graf.agents_count());
  for (auto j : rand_index) {
    graf.agents[j].infect(0);
  }
}

void Simulator::iterate(Results& results, Graph& graf, int t) {
  vac_strat->vaccinate(graf, t);
  int current_id = 0;
  int current_region = -1;
  for (Agent &agent : graf.agents) {
    if (graf.get_agent_region(current_id++) != current_region) {
      current_region++;
      results.prepare_new_region();
    }
    agent.update_results(t, results);
    if (agent.is_infected(t)) {
      if (agent.is_travelling(t)) {
        agent.try_infecting_on_travel(t, graf);
      }
      else {
        if (select_all)
          agent.try_infecting_neighbors(t, graf);
        else
          agent.try_infecting_n_neighbors(t, graf);
      }
      agent.update_infection(t);
    }
    else if (agent.is_vaccinated_susceptible(t)) {
      agent.update_vaccination(t);
    }
  }
}

Results Simulator::simulate(Graph& graf, bool print_each_result, bool randomize_sim) {
  Results results;
  infect_initial(graf, initial_infections);

  if (randomize_sim)
    seed_generator(time(NULL));
  else
    seed_generator(5);
  
  for (int t = 1; t <= (int)T; t++) {
    results.prepare_new_result();
    iterate(results, graf, t);
    if (print_each_result)
      results.write_last_to_output(std::cout, true);
  }
  return results;
}

Results Simulator::simulate(Graph& graf) {
  return simulate(graf, false, false);
}
