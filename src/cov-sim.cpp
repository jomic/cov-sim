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
  while ((c = getopt(argc, argv, "iops")) != -1) {
    switch (c) {
    case 'i':
      input_settings = true;
      break;
    case 'o':
      output_results = true;
      break;
    case 'p':
      print_graph = true;
      break;
    case 's':
      randomize_seed = true;
      break;
    }
  }

  // Initialize a seed for the randomizer:
  if (randomize_seed)
    srand(time(NULL));
  else
    srand(1);

  vector<shared_ptr<group_t>> groups;
  Graph edges;
  shared_ptr<VaccinationStrategy> vs = make_shared<NothingStrategy>();
  Simulator s(vs);

  // Load groups and settings from stream
  if (input_settings) {
    get_strategy_from_stream(cin, vs);
    s = Simulator(vs);
    reset_stream(cin);
    get_groups_from_stream(cin, groups);
    reset_stream(cin);
    initialize_simulator_from_stream(cin, s);
    reset_stream(cin);
    initialize_graph_from_stream(cin, edges);
  }
  else {
    edges.default_graph();
    edges.default_region_connections();
  }

  // Assign groups to the agents in the graph
  edges.assign_groups(groups);

  // Run the simulation
  Results results = s.simulate(edges, output_results);

  // Output results
  if (print_graph)
    results.print(10000);
}
