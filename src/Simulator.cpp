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
  for (Agent &node : edges.node_values) {
    node.update_results(t, results);
    if (node.is_infected(t)) {
      node.try_infecting_neighbours(t, edges);
      node.update_infection(t);
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
