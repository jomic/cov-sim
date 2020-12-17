#pragma once

#include "VaccinationStrategy.hpp"
#include "Graph.hpp"

class RandomStrategy: public VaccinationStrategy {
  
  void vaccinate(Graph& g, int t);
};

class NothingStrategy: public VaccinationStrategy {
public:
  void vaccinate(Graph& g, int t);
};
