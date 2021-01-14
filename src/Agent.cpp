#include <memory>
#include <vector>
#include "Agent.hpp"
#include "Graph.hpp"
#include "Results.hpp"
#include "Utils.hpp"

void Agent::try_completing_vaccination() {
  bool success = sample_bernoulli(group->p_v);
  if (success) {
    is_vaccined = true;
    is_susceptbl = false;
  }
}

void Agent::try_infecting_neighbor(int t, int target_id, Graph& graf) {
  Agent& agent = graf.agents[target_id];
  if (agent.is_susceptible(t)) {
    float risk;
    if (is_infectd) {
      risk = group->p_i * agent.group->susceptibility;
    } else {
      risk = group->p_ai * agent.group->susceptibility;
    }
    bool success = sample_bernoulli(risk);
    if (success)
      agent.infect(t);
  }
}

Agent::Agent(int id, std::shared_ptr<Group> group) : id(id), group(group) {
  is_susceptbl = true;
  is_asymptom = false;
  is_infectd = false;
  is_vaccined = false;
  is_recovrd = false;
  infected_on = -1;
  vaccinated_on = -1;
}

Group Agent::default_group;
Agent::Agent(int id) :
    Agent::Agent(id, std::make_shared<Group>(default_group)) {}
Agent::Agent() : Agent::Agent(id) {}

void Agent::assign_group(std::shared_ptr<Group>& new_group) {
  group = new_group;
}

bool Agent::can_be_vaccinated(int t) {
  return is_susceptbl && vaccinated_on == -1;
}

bool Agent::is_infected(int t) {
  return (is_infectd || is_asymptom) && t > infected_on;
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

bool Agent::is_travelling(int t) {
  float p = is_infectd ? group->p_t : group->p_at;
  return sample_bernoulli(p);
}

void Agent::infect(int t) {
  if (is_susceptbl) {
    bool success = sample_bernoulli(group->a_p);
    if (!success) {
      is_infectd = true;
    } else {
      is_asymptom = true;
    }
    infected_on = t;
    is_susceptbl = false;
  }
}

string Agent::to_string() { return std::to_string(id); }

void Agent::try_infecting_neighbors(int t, Graph& graf) {
  if (!is_infected(t))
    return;
  std::vector<int> n_ids = graf.neighbours(id);
  for (auto n_id : n_ids)
    try_infecting_neighbor(t, n_id, graf);
}

void Agent::try_infecting_n_neighbors(int t, Graph& graf) {
  if (!is_infected(t))
    return;
  std::vector<int> n_ids = graf.neighbours(id);
  int attempts = is_infectd ? group->n_i : group->n_ai;
  for (int j = 0; j < attempts; j++) {
    int target = sample_nonnegative(n_ids.size());
    try_infecting_neighbor(t, n_ids[target], graf);
  }
}

void Agent::try_infecting_on_travel(int t, Graph& graf) {
  int region = graf.get_agent_region(id);
  std::vector<int> candidate_regions = graf.get_neighboring_regions(region);
  if (candidate_regions.size() > 0) {
    int destination_index = sample_nonnegative(candidate_regions.size());
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
      int target_id = sample_nonnegative(n_potential_targets + dst_start);
      try_infecting_neighbor(t, target_id, graf);
    }
  }
}

void Agent::update_infection(int t) {
  if (is_infectd && infected_on + group->d_i < t) {
    is_infectd = false;
    is_recovrd = true;
  } else if (is_asymptom && infected_on + group->d_ai < t) {
    is_asymptom = false;
    is_recovrd = true;
  }
}

void Agent::update_vaccination(int t) {
  if (is_vaccinated_susceptible(t) && vaccinated_on + group->d_v + 1 == t) {
    try_completing_vaccination();
  }
}

void Agent::update_results(int t, Results& results) {
  if (is_susceptbl || ((is_infectd || is_asymptom) && t == infected_on))
    results.add_susceptible();
  else if (is_asymptom)
    results.add_asymptomatic();
  else if (is_infectd)
    results.add_infected();
  else if (is_vaccined)
    results.add_vaccinated();
  else if (is_recovrd)
    results.add_removed();
}

void Agent::vaccinate(int t) {
  vaccinated_on = t;
}
