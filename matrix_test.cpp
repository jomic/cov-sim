#include "Graph.hpp"
#include <iostream>
#include "Simulator.hpp"
#include "Results.hpp"




int main()
{

  srand(time(NULL));
  Graph g;
  g.matrix_graph(10, 2);



  Simulator s;
  settings_t settings;

  Results results = s.simulate(settings, g);
  results.print();
  //  g.print_graph();
  return 0;
}
