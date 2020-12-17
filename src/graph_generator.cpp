#include <cmath>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "Graph.hpp"
using namespace std;

int main(int argc, char** argv) {
  srand(time(NULL));
  bool add_region = false;
  bool output_result = false;
  int l = 10;
  int k = 1;
  float p = 0.0f;
  
  int c;
  while ((c = getopt(argc, argv, "rol:k:p:")) != -1) {
    switch (c) {
    case 'r':
      add_region = true;
      break;
    case 'o':
      output_result = true;
      break;
    case 'l':
      l = atoi(optarg);
      break;
    case 'k':
      k = atoi(optarg);
      break;
    case 'p':
      p = atof(optarg);
      break;
    }
  }

  // Calculate and print some graph info
  // Path length estimation based on Newman, Moore and Watts (1999)
  float expected_degree = 2 * k + p * (l - 2 * k - 1);
  float z = 1 / (k * p);
  float sqrt_term = sqrt(1 + 2 * z / l);
  float expected_path_length = atanh(1 / sqrt_term) * z / (2 * k * sqrt_term);
  clog << "== Graph data ==" << endl
       << "Number of nodes:\t" << l << endl
       << "Expected degree:\t" << expected_degree << endl
       << "Expected path length:\t" << expected_path_length << endl
       << "Note that the path length may be inaccurate for some values." << endl
       << "================" << endl << endl;

  Graph g;
  if (add_region)
    g.read_generatable_graph(cin);
  g.nw_small_world(l, k, p);
  if (output_result)
    g.write_generatable_graph(cout);
}
