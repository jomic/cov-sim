#pragma once

class Graph;
#include "Results.hpp"

class Agent {
private:
  int id;
  bool s;
  bool i;
  bool r;
  float susceptibility;
  int infected_on;

  void try_infecting_neighbour(int t, int id, Graph& edges);

public:
  Agent(int id, float susceptibility);
  Agent(int id);

  /*
    Check if the individual can infect people at time t.
  */
  bool is_infected(int t);

  /*
    Check if the individual can still be infected at time t.
  */
  bool is_susceptible(int t);

  /*
    Infect the individual at time t.
  */
  void infect(int t);

  /*
    Make the individual try to infect all of its neighbours at time t.
  */
  void try_infecting_neighbours(int t, Graph& edges);

  /*
    Update the individuals infection, given that an infection last for days_sick time steps.
  */
  void update_infection(int t, int days_sick);
  
  /*
    Update the results with the individual.
  */
  void update_results(int t, Results& results);
};
