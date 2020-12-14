#pragma once

class Graph;
#include <memory>
#include "Results.hpp"
#include "Group.hpp"

class Agent {
private:
  int id;
  bool s;
  bool a;
  bool i;
  bool v;
  bool r;
  int infected_on;
  std::shared_ptr<group_t> group;
  static group_t default_group;

  void try_infecting_neighbour(int t, int id, Graph& edges);

public:
  Agent(int id, std::shared_ptr<group_t> group);
  Agent(int id);

  /*
    Assign a new group to the agent.
   */
  void assign_group(std::shared_ptr<group_t>& new_group);
  
  /*
    Check if the individual can infect people at time t.
  */
  bool is_infected(int t);

  /*
    Check if the individual can still be infected at time t.
  */
  bool is_susceptible(int t);

  /*
    Check if the individual is travelling at the time t.
   */
  bool is_travelling(int t);

  /*
    Infect the individual at time t.
  */
  void infect(int t);

  /*
    Make the individual try to infect all of its neighbours at time t.
  */
  void try_infecting_neighbours(int t, Graph& edges);

  /*
    Make the individual try to infect some of its neighbours at time t, n based on its group params.
  */
  void try_infecting_n_neighbours(int t, Graph& edges);

  /*
    Make the individual try to infect n people in a neighbouring region, based on group params.
   */
  void try_infecting_on_travel(int t, Graph& edges);
  
  /*
    Update the individuals infection, given that an infection last for d time steps.
  */
  void update_infection(int t);
  
  /*
    Update the results with the individual.
  */
  void update_results(int t, Results& results);
};
