#include <iostream>
#include <vector>
#include "Graph.hpp"
#include "IOjson.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
using namespace std;

int main(int argc, char** argv) {
  // TODO: Flags maybe? Right now just checks if any arg exists
  bool input_settings = argc > 1;
  
  // Initialize a seed for the randomizer:
  srand(1);
  // srand(time(NULL));

  vector<group_t> groups;
  Simulator s;
  Graph edges;

  // Load groups and settings from stream:
  if (input_settings) {
    get_groups_from_stream(cin, groups);
    reset_stream(cin);
    initialize_simulator_from_stream(cin, s);
    reset_stream(cin);
    initialize_graph_from_stream(cin, edges, groups);
  }
  else {
    edges.default_graph();
  }
  
  clog << "Number of groups: " << groups.size() << endl;
  // edges.input_from_file("ex_input_1.txt");
  // edges.print_graph();
  Results results = s.simulate(edges);
  // results.write_to_output_stream(cout);
  results.print(49);
}
