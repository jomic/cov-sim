#pragma once

#include "VaccinationStrategy.hpp"
#include "Graph.hpp"

class RandomStrategy: public VaccinationStrategy {
public:
  int time_deployed{0};
  int success_rate{0};
  int vaccines_per_day{1};
  
  void vaccinate(Graph& g);
};

class NothingStrategy: public VaccinationStrategy {
public:
  void vaccinate(Graph& g);
};
