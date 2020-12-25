#pragma once
#include "Graph.hpp"
#include "VcStrgy.hpp"

class RandomStrategy: public VcStrgy {
  void vaccinate(Graph& graf, int t);
};

class NothingStrategy: public VcStrgy {
public:
  void vaccinate(Graph& graf, int t);
};
