#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include "Graph.hpp"
#include "Utils.hpp"
#include "VacStrats.hpp"

void RandomStrategy::vaccinate(Graph& graf, int t) {

  if (t < time_deployed) {
    return;
  }

  vector<int> unvaccinated;

  for (int i = 0; i < graf.node_count(); ++i) {
    if (graf.get_node(i).can_be_vaccinated(t)) {
      unvaccinated.push_back(i);
    }
  }

  if(unvaccinated.size() == 0) return;

  int unvaccinated_count = unvaccinated.size();
  int vaccines = std::min(vaccines_per_day, unvaccinated_count);

  set<int> nums = unique_random_numbers(vaccines, unvaccinated.size());

  for (int i : nums) {
    graf.agents[i].vaccinate(t);
  }

  return;
}

void NothingStrategy::vaccinate(Graph& graf, int t) {
  return;
}
