#pragma once
#include "Results.hpp"
#include "Graph.hpp"

struct settings_t {
  int t_end{50};
  int initial_infections{1};
};

class Simulator {
private:
  void infect_initial(Graph& edges, int n);
public:
  void iterate(Results& result, Graph& edges, int t);
  Results simulate(settings_t& settings, Graph& edges);
};
