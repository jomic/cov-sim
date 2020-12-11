#pragma once

#include "Graph.hpp"

class VaccinationStrategy {
public:
  virtual void vaccinate(Graph& g) = 0;
};
