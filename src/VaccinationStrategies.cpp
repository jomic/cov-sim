#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include "Graph.hpp"
#include "Utilities.hpp"
#include "VaccinationStrategies.hpp"

void RandomStrategy::vaccinate(Graph& g, int t) {

  if (t < time_deployed) {
    return;
  }
  
  vector<int> unvaccinated;

  for (int i = 0; i < g.node_count(); ++i) {
    if (g.get_node(i).can_be_vaccinated(t)) {
      unvaccinated.push_back(i);
    }
  }
  
  if(unvaccinated.size() == 0) return;

  int unvaccinated_count = unvaccinated.size();
  int vaccines = std::min(vaccines_per_day, unvaccinated_count);
  
  set<int> nums = unique_random_numbers(vaccines,
                                        unvaccinated.size());

  for (int i : nums) {
    g.node_values[i].vaccinate(t);
  }
  
  return;
}

void NothingStrategy::vaccinate(Graph& g, int t) {
  return;
}
