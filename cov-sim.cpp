#include <iostream>
#include "Simulator.hpp"
#include "Results.hpp"
#include "Graph.hpp"


int main() {
  // Initialize a seed for the randomizer
  srand(time(NULL));
  
  Graph edges;
  edges.input_from_file("ex_input_1.txt");
  edges.print_graph();

  Simulator s;
  settings_t settings;

  Results results = s.simulate(settings, edges);
  results.print();
}
