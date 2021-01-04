#include <cmath>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "Graph.hpp"
using namespace std;

int main(int argc, char** argv) {
  bool add_region = false, output_result = false, randomized_seed = false;
  int N = 10, c = 4;

  int ch;
  while ((ch = getopt(argc, argv, "orsN:c:")) != -1) {
    switch (ch) {
    case 'o':
      output_result = true;
      break;
    case 'r':
      add_region = true;
      break;
    case 's':
      randomized_seed = true;
      break;
    case 'N':
      N = atoi(optarg);
      break;
    case 'c':
      c = atof(optarg);
      break;
    }
  }

  // Initialize a seed for the randomizer:
  if (randomized_seed) { srand(time(NULL)); }
  else { srand(0); }

  Graph graf;
  if (add_region) { graf.read_generatable_graph(cin); }
  graf.random_graph(N,c);
  if (N<11) { graf.print_agents_edges_offsets(" random_graph:"); }
  if (output_result) { graf.write_generatable_graph(cout); }
}
