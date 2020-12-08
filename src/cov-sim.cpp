#include <experimental/filesystem>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "Initializer.hpp"
using namespace std;

int main() {
  cout << "path = " << experimental::filesystem::current_path() << endl;
  // Initialize a seed for the randomizer:
  srand(1);
  // srand(time(NULL));

  vector<group_t> groups;
  Simulator s;

  // Load groups and settings from stream
  get_groups_from_stream(cin, groups);
  reset_stream(cin);
  initialize_simulator_from_stream(cin, s);

  cout << "Number of groups: " << groups.size() << endl;
  Graph edges;
  edges.input_from_file("ex_input_1.txt");
  edges.print_graph();

  Results results = s.simulate(edges);
  results.print(8);
}
