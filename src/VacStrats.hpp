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

class PriorityStrategy: public VacStrat {
protected:
  virtual int get_priority(Graph& graf, int id, int t);
public:
  void vaccinate(Graph& graf, int t);
};

class DensityStrategy: public PriorityStrategy {
protected:
  int get_density(Graph& graf, int id, int t);
};

class LowestDensityStrategy: public DensityStrategy {
protected:
  int get_priority(Graph& graf, int id, int t);
};

class HighestDensityStrategy: public DensityStrategy {
protected:
  int get_priority(Graph& graf, int id, int t);
};
