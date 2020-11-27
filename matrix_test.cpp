#include "Graph.hpp"
#include <iostream>
#include "Simulator.hpp"
#include "Results.hpp"

using namespace std;

int main(int argc, char *argv[]) {

  int L = 10;
  int D_zero = 5;
  int t = 50;

  if (argc == 4) {
    L = stoi(argv[1]);
    D_zero = stoi(argv[2]);
    t = stoi(argv[3]);
  } else if (argc != 1) {
    cout << "Usage: " << argv[0] << " L D0 t\n";
    return 1;
  }
  
  srand(time(NULL));
  Graph g;
  g.matrix_graph(L, D_zero);

  Simulator s;
  settings_t settings;
  settings.t_end = t;

  Results results = s.simulate(settings, g);
  results.print();
  //  g.print_graph();
  return 0;
}
