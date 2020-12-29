#include <memory>
#include <vector>
#include "Agent.hpp"
#include "Graph.hpp"
#include "Results.hpp"

Group Agent::default_group;

void Agent::try_infecting_neighbour(int t, int target_id, Graph& graf) {
  Agent& agent = graf.agents[target_id];
  if (agent.is_susceptible(t)) {
    float roll = (float)rand() / (float)RAND_MAX;
    float risk;
    if (is_infectd) {
      risk = group->p_i * agent.group->susceptibility;
    } else {
      risk = group->p_ai * agent.group->susceptibility;
    }
    if (roll < risk)
      agent.infect(t);
  }
}

void Agent::try_completing_vaccination() {
  float roll = (float)rand() / (float)RAND_MAX;
  float risk = group->p_v;
  if (roll < risk) {
    is_vaccined = true;
    is_susceptbl = false;
  }
}

Agent::Agent(int id, std::shared_ptr<Group> group)
  : id(id), group(group) {
  is_susceptbl = true;
  is_asympt = false;
  is_infectd = false;
  is_vaccined = false;
  is_recovrd = false;
  infected_on = -1;
  vaccinated_on = -1;
}

Agent::Agent(int id)
  : Agent::Agent(id, std::make_shared<Group>(default_group)) {}

void Agent::assign_group(std::shared_ptr<Group>& new_group) {
  group = new_group;
}

bool Agent::is_infected(int t) {
  return (is_infectd || is_asympt) && t > infected_on;
}

bool Agent::is_susceptible(int t) {
  return is_susceptbl;
}

bool Agent::is_vaccinated() {
  return is_vaccined;
}

bool Agent::is_vaccinated_susceptible(int t) {
  return (is_susceptbl && (vaccinated_on != -1));
}

bool Agent::can_be_vaccinated(int t) {
  return is_susceptbl && vaccinated_on == -1;
}

bool Agent::is_travelling(int t) {
  float p = is_infectd ? group->p_t : group->p_at;
  return p > (float) rand() / (float) RAND_MAX;
}

void Agent::infect(int t) {
  if (is_susceptbl) {
    is_susceptbl = false;
    float roll = (float)rand() / (float)RAND_MAX;
    if (roll >= group->a_p) {
      is_infectd = true;
    } else {
      is_asympt = true;
    }
    infected_on = t;
  }
}

void Agent::vaccinate(int t) {
  vaccinated_on = t;
}

void Agent::try_infecting_neighbours(int t, Graph& graf) {
  if (!is_infected(t))
    return;
  std::vector<int> n_ids = graf.neighbours(id);
  for (auto n_id : n_ids)
    try_infecting_neighbour(t, n_id, graf);
}

void Agent::try_infecting_n_neighbours(int t, Graph& graf) {
  if (!is_infected(t))
    return;
  std::vector<int> n_ids = graf.neighbours(id);
  int attempts = is_infectd ? group->n_i : group->n_ai;
  for (int j = 0; j < attempts; j++) {
    int target = rand() % n_ids.size();
    try_infecting_neighbour(t, n_ids[target], graf);
  }
}

void Agent::try_infecting_on_travel(int t, Graph& graf) {
  int region = graf.get_agent_region(id);
  std::vector<int> candidate_regions = graf.get_neighbouring_regions(region);
  if (candidate_regions.size() > 0) {
    int destination_index = rand() % candidate_regions.size();
    int destination = candidate_regions[destination_index];
    int dst_start = graf.region_agent_offsets[destination];
    int dst_end;
    if (destination == (int) graf.region_agent_offsets.size() - 1)
      dst_end = graf.agents.size();
    else
      dst_end = graf.region_agent_offsets[destination + 1];
    int n_potential_targets = dst_end - dst_start;
    int n_attempts = is_infectd ? group->n_i : group->n_ai;
    for (int j = 0; j < n_attempts; j++) {
      int target_id = rand() % n_potential_targets + dst_start;
      try_infecting_neighbour(t, target_id, graf);
    }
  }
}

void Agent::update_infection(int t) {
  if (is_infectd && infected_on + group->d_i < t) {
    is_infectd = false;
    is_recovrd = true;
  } else if (is_asympt && infected_on + group->d_ai < t) {
    is_asympt = false;
    is_recovrd = true;
  }
}

void Agent::update_vaccination(int t) {
  if (is_vaccinated_susceptible(t) && vaccinated_on + group->d_v + 1 == t) {
    try_completing_vaccination();
  }
}

void Agent::update_results(int t, Results& results) {
  if (is_susceptbl || ((is_infectd || is_asympt) && t == infected_on))
    results.add_susceptible();
  else if (is_asympt)
    results.add_asymptomatic();
  else if (is_infectd)
    results.add_infected();
  else if (is_vaccined)
    results.add_vaccinated();
  else if (is_recovrd)
    results.add_removed();
}
