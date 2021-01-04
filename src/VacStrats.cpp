#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>
#include "Graph.hpp"
#include "Utils.hpp"
#include "VacStrats.hpp"

void RandomStrategy::vaccinate(Graph& graf, int t) {

  if (t < time_deployed) {
    return;
  }

  vector<int> unvaccinated;

  for (int j = 0; j < graf.agents_count(); ++j) {
    if (graf.get_agent(j).can_be_vaccinated(t)) {
      unvaccinated.push_back(j);
    }
  }

  if(unvaccinated.size() == 0) return;

  int unvaccinated_count = unvaccinated.size();
  int vaccines = std::min(vaccines_per_day, unvaccinated_count);

  set<int> nums = unique_random_numbers(vaccines, unvaccinated.size());

  for (int j : nums) {
    graf.agents[j].vaccinate(t);
  }

  return;
}

void NothingStrategy::vaccinate(Graph& graf, int t) {
  return;
}

void DensityStrategy::vaccinate(Graph& graf, int t) {
  if (t < time_deployed)
    return;
  
  priority_queue<pair<int, int>> targets;
  
  for (int i = 0; i < graf.agents_count(); i++) {
    if (graf.agents[i].can_be_vaccinated(t)) {
      int priority = get_priority(graf, i, t);
      targets.push(make_pair(priority, i));
    }
  }
  
  int n_vacs = std::min((int) targets.size(), vaccines_per_day);
  for (int i = 0; i < n_vacs; i++) {
    int target_id = targets.top().second;
    graf.agents[target_id].vaccinate(t);
    targets.pop();
  }
}

int DensityStrategy::get_density(Graph& graf, int id, int t) {
  int density = 0;
  vector<int> ns = graf.neighbours(id);
  for (int n : ns) {
    if (graf.agents[n].is_infected(t))
      density++;
  }
  return density;
}

int DensityStrategy::get_priority(Graph& graf, int id, int t) {
  return 0;
}

int HighestDensityStrategy::get_priority(Graph& graf, int id, int t) {
  int density = get_density(graf, id, t);
  return density;
}

int LowestDensityStrategy::get_priority(Graph& graf, int id, int t) {
  int density = get_density(graf, id, t);
  return -density;
}
