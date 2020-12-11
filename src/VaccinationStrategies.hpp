#pragma once

#include "VaccinationStrategy.hpp"
#include "Graph.hpp"

class TrivialStrategy: public VaccinationStrategy {
public:
  void vaccinate(Graph& g);
};

class NothingStrategy: public VaccinationStrategy {
public:
  void vaccinate(Graph& g);
};
