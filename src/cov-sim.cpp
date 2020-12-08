#include <iostream>
#include <vector>
#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "IOStreamHandler.hpp"
using namespace std;

int main(int argc, char** argv) {
  // TODO: Flags maybe? Right now just checks if any arg exists
  bool input_settings = argc > 1;
  
  // Initialize a seed for the randomizer:
  srand(1);
  // srand(time(NULL));

  vector<group_t> groups;
  Simulator s;

  // Load groups and settings from stream
  if (input_settings) {
    get_groups_from_stream(cin, groups);
    reset_stream(cin);
    initialize_simulator_from_stream(cin, s);
  }
  
  clog << "Number of groups: " << groups.size() << endl;
  Graph edges;
  edges.input_from_file("ex_input_1.txt");
  edges.print_graph();
  Results results = s.simulate(edges);
  results.write_to_output_stream(cout);
}
