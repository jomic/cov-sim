#pragma once

#include "Graph.hpp"

class VaccinationStrategy {
public:
  float success_rate{0};    // probability of vaccination working
  int time_deployed{0};     // time when the vaccination is deployed
  int vaccines_per_day{1};  // how many vaccinations per day
  int affect_time{28};      // how long before vaccination starts working
  
  virtual void vaccinate(Graph& g, int t) = 0;
};
