#pragma once
#include <iostream>
#include <memory>
#include "Graph.hpp"
#include "IOjson.hpp"
#include "Results.hpp"
#include "VcStrgies.hpp"
#include "VcStrgy.hpp"
using namespace std;

class Simulator {
private:
/**
  @random_seed        True if the seed is random, else false.
  @select_all         True if an infectious tries to infect all neighbours.
  @betaC              Probability that Covid-19 transmits when two people meet.
  @initial_infections Number of initially infected agents.
  @L                  The square root of the population.
  @days_sick          How long is one person sick.
  @D0                 Grid size; number of neighbours = (2·D0+1)^2 - 1.
  @T0                 The time step when a lockdown is put forth.
  @T                  Number of time steps the simulation iterates.
  @T_v                The time step when vaccination starts.
  @n_v                Number of vaccines available per timestep.
*/ // Parameters set to their default values:
  bool random_seed{false};
  bool select_all{true};
  float betaC{0.25};
  unsigned int initial_infections{4};
  unsigned int L{7};
  unsigned int days_sick{14};
  unsigned int D0{2};
  unsigned int T0{60};
  unsigned int T{20};
  unsigned int T_v{10};
  unsigned int n_v{10};

  shared_ptr<VcStrgy> vac_strat;

  /**
    Infects n agents at time t=0.
   */
  void infect_initial(Graph& graf, int n);

public:
  Simulator(shared_ptr<VcStrgy>& vs);

  /**
    Performs a single iteration at time t.
   */
  void iterate(Results& results, Graph& graf, int t);

  /**
    Runs a simulation for given settings for the specified graph `graf`.
   */
  Results simulate(Graph& graf, bool print_each_result);
  Results simulate(Graph& graf);

  // Allow the initializer function to access private members
  friend void initialize_simulator_from_stream(istream& stream, Simulator& s);
};
