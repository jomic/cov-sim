#pragma once
#include "Graph.hpp"

class VaccinationStrategy {
public:
  int time_deployed{0};     // time when the vaccination is deployed
  int vaccines_per_day{1};  // how many vaccinations per day
  
  virtual void vaccinate(Graph& graf, int t) = 0;
};
