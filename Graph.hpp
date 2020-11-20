#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include "Individual.hpp"

using namespace std;

class Relation {
public:
  int distance;

  Relation(int d);
};

class Graph {
public:
  vector<int> offsets;
  vector<int> edges;
  vector<Relation> edge_values;
  vector<Individual> node_values;

  void input_from_file(string file_name);

  vector<int> neighbours(int id);

  int node_count();

  Individual get_node(int id);

  void print_graph();
};

#endif
