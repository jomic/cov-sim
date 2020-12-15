#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include <vector>
#include <set>
#include <iostream>

std::set<int> unique_random_numbers(int n, int max) {
  std::set<int> numbers;
  if (n > max) {
    std::cout << "random_numbers: n cant be larger than total\n";
    return numbers;
  }
  while ((int)numbers.size() < n) {
      numbers.insert(rand() % max);
    }
  return numbers;
}

void Simulator::infect_initial(Graph& edges, int n) {
  std::set<int> rand_index = unique_random_numbers(n, edges.node_count());
  for (auto i : rand_index) {
    edges.node_values[i].infect(0);
  }
}

void Simulator::iterate(Results& results, Graph& edges, int t) {
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
  }
}

Results Simulator::simulate(Graph& edges, bool print_each_result) {
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
    if (print_each_result)
      results.write_last_to_output_stream(std::cout);
  }
  return results;
}

Results Simulator::simulate(Graph& edges) {
  return simulate(edges, false);
}
