#include <experimental/filesystem>
#include <iostream>
#include <unistd.h>
#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "Initializer.hpp"
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
  initialize_simulator_from_stream(cin, s);

  Results results = s.simulate(edges);
  results.print(8);
}
