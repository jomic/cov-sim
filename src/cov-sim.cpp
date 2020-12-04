#include <iostream>
#include <experimental/filesystem>
#include <unistd.h>
#include "Simulator.hpp"
#include "Results.hpp"
#include "Graph.hpp"
using namespace std;

int main() {
  cout << "path = " << experimental::filesystem::current_path() << endl;
  Graph edges;
  edges.input_from_file("ex_input_1.txt");
  edges.print_graph();

  // Initialize a seed for the randomizer:
  srand(1);
  // srand(time(NULL));

  Simulator s;

  Results results = s.simulate(edges);
  results.print(8);
}
