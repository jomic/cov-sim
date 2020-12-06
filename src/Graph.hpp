#pragma once
#include <string>
#include <vector>
#include "Agent.hpp"

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
  vector<Agent> node_values;

  void input_from_file(string file_name);

  void matrix_graph(int size, int distance);

  vector<int> neighbours(int id);

  int node_count();

  Agent get_node(int id);

  void print_graph();
};
