#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

using namespace std;

class Individual {
public:
  int id;
  int susceptibility;
  int status = 0;

  Individual(int id, int s);
};

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

  int get_node_status(int id);

  int node_count();

  void print_graph();
};

#endif
