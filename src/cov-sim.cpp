#include <iostream>
#include <vector>
#include <memory>
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

  vector<shared_ptr<group_t>> groups;
  Simulator s;
  Graph edges;

  // Load groups and settings from stream
  if (input_settings) {
    get_groups_from_stream(cin, groups);
    reset_stream(cin);
    initialize_simulator_from_stream(cin, s);
    reset_stream(cin);
    initialize_graph_from_stream(cin, edges);
  }
  else {
    edges.default_graph();
  }

  // Assign groups to the agents in the graph
  edges.assign_groups(groups);
  
  // edges.print_graph();
  Results results = s.simulate(edges);
  results.write_to_output_stream(cout);
}
