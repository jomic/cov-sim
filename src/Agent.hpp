#pragma once
#include <memory>
class Graph;
#include "Results.hpp"
#include "Group.hpp"
using namespace std;

class Agent {
private:
  int id;
  bool is_susceptbl;
  bool is_asymptom;
  bool is_infectd;
  bool is_vaccined;
  bool is_recovrd;
  int infected_on;
  int vaccinated_on;
  std::shared_ptr<Group> group;
  static Group default_group;

  void try_completing_vaccination();
  void try_infecting_neighbor(int t, int id, Graph& graf);

public:
  Agent(int id, std::shared_ptr<Group> group);
  Agent(int id);
  Agent();

  /**
    Assign a new group to the agent.
   */
  void assign_group(std::shared_ptr<Group>& new_group);

  /**
    Check if the agent is eligible for vaccination at time t.
   */
  bool can_be_vaccinated(int t);

  /**
    Check if the individual can infect people at time t.
  */
  bool is_infected(int t);

  /**
    Check if the individual can still be infected at time t.
  */
  bool is_susceptible(int t);

  /**
    Check if the individual is vaccinated at time t.
  */
  bool is_vaccinated();

  /**
    Check if the individual is vaccinated but still susceptible at time t.
  */
  bool is_vaccinated_susceptible(int t);

  /**
    Check if the individual is travelling at the time t.
   */
  bool is_travelling(int t);

  /**
    Infect the individual at time t.
  */
  void infect(int t);

  /**
    Return the agent's *id* as a string.
  */
  string to_string();

  /**
    Make the individual try to infect all of its neighbours at time t.
  */
  void try_infecting_neighbors(int t, Graph& graf);

  /**
    Make the individual try to infect some of its neighbours at time t,
    based on its group parameters.
  */
  void try_infecting_n_neighbors(int t, Graph& graf);

  /**
    Make the individual try to infect n people in a neighbouring region,
    based on its group parameters.
   */
  void try_infecting_on_travel(int t, Graph& graf);

  /**
    Update the individuals infection, given that an infection lasts
    for t time steps.
  */
  void update_infection(int t);

  /**
    Update the individuals vaccination.
  */
  void update_vaccination(int t);

  /**
    Update the results with the individual.
  */
  void update_results(int t, Results& results);

  /**
    Vaccinate the individual
  */
  void vaccinate(int t);

  void tell_reporter_connections(Graph& graf);
};
