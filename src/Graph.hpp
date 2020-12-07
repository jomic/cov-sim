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

  /* Read node information from file.
     Format of file should be:
     NODE_ID CONNECTED_TO_1 CONNECTED_TO_2 ...
     on each line of the file
     Example:
     0 1 2 3
     1 0 2
     2 0 1
     3 0
  */
  void input_from_file(string file_name);

  /* Create a graph with dimensions *size*
     with same connection as an matrix with
     distance *distance*.
  */
  void matrix_graph(int size, int distance);

  /* Returns list of neighbours of node *id* */
  vector<int> neighbours(int id);

  /* Returns amount of nodes */
  int node_count();

  Agent get_node(int id);


  /* Prints information about the graph */
  void print_graph();
};
