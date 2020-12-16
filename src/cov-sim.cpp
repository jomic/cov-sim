#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "IOStreamHandler.hpp"
using namespace std;

int main(int argc, char** argv) {
  bool input_settings = false;
  bool output_results = false;
  bool randomize_seed = false;
  bool print_graph = false;
  
  // Handle flags
  int c;
  while ((c = getopt(argc, argv, "iorp")) != -1) {
    switch (c) {
    case 'i':
      input_settings = true;
      break;
    case 'o':
      output_results = true;
      break;
    case 'r':
      randomize_seed = true;
      break;
    case 'p':
      print_graph = true;
      break;
    }
  }
  
  // Initialize a seed for the randomizer:
  if (randomize_seed)
    srand(time(NULL));
  else
    srand(1);

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

  // Run the simulation
  Results results = s.simulate(edges);

  // Output results
  if (print_graph)
    results.print(10000);
  if (output_results)
    results.write_to_output_stream(cout);
}
