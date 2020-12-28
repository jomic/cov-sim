#pragma once
#include "Graph.hpp"
#include "VacStrat.hpp"

class RandomStrategy: public VacStrat {
  void vaccinate(Graph& graf, int t);
};

class NothingStrategy: public VacStrat {
public:
  void vaccinate(Graph& graf, int t);
};
