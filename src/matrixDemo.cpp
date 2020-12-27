#include <iostream>
#include "Graph.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
using namespace std;

int main(int argc, char *argv[]) {

  int L = 10;
  int D0 = 5;

  if (argc == 3) {
    L = stoi(argv[1]);
    D0 = stoi(argv[2]);
  } else if (argc != 1) {
    cout << "Usage: " << argv[0] << " L D0 t\n";
    return 1;
  }

  srand(1);
  Graph graph;
  graph.matrix_graph(L, D0);
  shared_ptr<VcStrgy> vs = make_shared<NothingStrategy>();
  Simulator s(vs);
  Results results = s.simulate(graph);
  results.save_to_file("data.txt");
  results.plot();
  return 0;
}
