#include <iostream>
#include <cstdlib>
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

  Graph g;
  if (add_region)
    g.read_generatable_graph(cin);
  g.nw_small_world(l, k, p);
  if (output_result)
    g.write_generatable_graph(cout);
}
