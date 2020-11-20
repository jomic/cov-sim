#pragma once

struct settings_t {
  int t_end{50};
};

class Simulator {
public:
  void iterate(Result& result, Grid& edges, int t);
  Result simulate(settings_t& settings, Grid& edges);
};
