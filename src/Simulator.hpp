#pragma once
#include "Graph.hpp"
#include "Results.hpp"

class Simulator {
private:
/**
  @initial_infections number of initially infected agents.
  @L                  the square root of the population.
  @days_sick          how long is one person sick.
  @D0                 the square root of the population.
  @T0                 the time step when a lockdown is put forth.
  @t_end              number of time steps the simulation iterates.
  @betaC              probability that Covid-19 transmits when two people meet.
  @random_seed        true if the seed is random, else false.
*/ // Parameters set to their default values:
  int initial_infections = 4;
  int L = 7;
  int days_sick = 14;
  int D0 = 2;
  int T0 = 60;
  int t_end = 60;
  float betaC = 0.25;
  bool random_seed = false;

  /**
    Infects n agents at time t=0.
   */
  void infect_initial(Graph& edges, int n);

public:
  /**
    Performs a single iteration at time t.
   */
  void iterate(Results& results, Graph& edges, int t);

  /**
    Runs a simulation for given settings for the specified graph `edges`.
   */
  Results simulate(Graph& edges);
};
