#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>
#include "Graph.hpp"
#include "IOStreamHandler.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
using namespace std;

int main(int argc, char** argv) {
  bool input_settings = false;
  bool output_results = false;
  bool realtime_output = false;
  bool randomize_seed = false;
  bool print_graph = false;

  // Handle flags
  int c;
  while ((c = getopt(argc, argv, "iotps")) != -1) {
    switch (c) {
    case 'i':
      input_settings = true;
      break;
    case 'o':
      output_results = true;
      break;
    case 't':
      realtime_output = true;
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
  Graph graf;
  shared_ptr<VaccinationStrategy> vs = make_shared<NothingStrategy>();
  Simulator s(vs);

  // Load groups and settings from stream
  if (input_settings) {
    get_strategy(cin, vs);
    s = Simulator(vs);
    reset_stream(cin);
    get_groups(cin, groups);
    reset_stream(cin);
    initialize_simulator_from_stream(cin, s);
    reset_stream(cin);
    initialize_graph_from_stream(cin, graf);
  }
  else {
    graf.default_graph();
    graf.default_region_connections();
  }

  // Assign groups to the agents in the graph
  graf.assign_groups(groups);

  // Run the simulation
  Results results = s.simulate(graf, output_results && realtime_output);

  // Output results
  if (print_graph)
    results.print(10000);
  if (output_results && !realtime_output)
    results.write_to_output_stream(cout, false);
}
