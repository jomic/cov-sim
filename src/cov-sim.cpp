#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>
#include "Graph.hpp"
#include "IOjson.hpp"
#include "Results.hpp"
#include "Simulator.hpp"

int main(int argc, char** argv) {
  bool input_settings = false;
  bool output_results = false;
  bool realtime_output = false;
  bool randomize_seed = false;
  bool plot_graph = false;

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
      plot_graph = true;
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
    srand(5);

  vector<shared_ptr<Group>> groups;
  Graph graf;
  shared_ptr<VacStrat> vs = make_shared<NothingStrategy>();
  Simulator sim(vs);

  // Load groups and settings from stream
  if (input_settings) {
    get_strategy(cin, vs);
    sim = Simulator(vs);
    reset_stream(cin);
    get_groups(cin, groups);
    reset_stream(cin);
    initialize_simulator(cin, sim);
    reset_stream(cin);
    initialize_graph(cin, graf);
  }
  else {
    graf.default_graph();
    graf.default_region_connections();
  }

  // Assign groups to the agents in the graph
  graf.assign_groups(groups);

  // Run the simulation
  Results results = sim.simulate(graf, output_results && realtime_output);

  // Output results:
  if (plot_graph) { results.plot(); }
  else if (!input_settings) {
    results.plot();
    graf.print_agents_edges_offsets(" RANDOM graph:");
  }
  if (output_results && !realtime_output)
    results.write_to_output(cout, false);
}
