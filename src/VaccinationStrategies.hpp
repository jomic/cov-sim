#pragma once
#include "Graph.hpp"
#include "VaccinationStrategy.hpp"

class RandomStrategy: public VaccinationStrategy {
  void vaccinate(Graph& graf, int t);
};

class NothingStrategy: public VaccinationStrategy {
public:
  void vaccinate(Graph& graf, int t);
};
