// CRS

#include "Graph.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

// https://stackoverflow.com/a/46931770
vector<string> split (string s, string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;

  while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}


Individual::Individual(int id, int s) : id(id), susceptibility(s) {}

Relation::Relation(int d) : distance(d) {}

void Graph::input_from_file(string file_name) {
  fstream newfile;
  newfile.open(file_name, ios::in);
  if (newfile.is_open()) {
    string tp;
    while(getline(newfile, tp)) {

      offsets.push_back(edges.size());
        
      vector<string> v = split(tp, " ");
      bool first = false;
      for (auto i : v) {
        if (!first) {
          first = true;
          // note: hardcoded susceptibility
          node_values.push_back(Individual(stoi(i), 1));
        } else {
          edges.push_back(stoi(i));
            
        }
      }
    }
    newfile.close();
  }
}

// return neighbours of node id
vector<int> Graph::neighbours(int id) {
  vector<int> neighbours;
  int start = offsets[id];
  int end;
  if (id + 1 < offsets.size()) {
    end = offsets[id + 1];
  } else {
    end = offsets.size();
  }

  for (int i = start; i < end;  i++) {
    neighbours.push_back(edges[i]);
  }

  return neighbours;
}

int Graph::get_node_status(int id) {
  return node_values[offsets[id]].status;
}

int Graph::node_count() {
  return offsets.size();
}

void Graph::print_graph() {
  cout << "Offsets: ";
  for (auto i : offsets) { cout << i << ", "; }
  cout << endl;
  cout << "Edges: ";
  for (auto i : edges) { cout << i << ", "; }
  cout << endl;
}

int main(int argc, char *argv[])
{
  Graph g;
  g.input_from_file("ex_input_1.txt");
  g.print_graph();
  vector<int> n = g.neighbours(0);
  cout << "Neighbour of node 0: ";
  for (auto i : n) { cout << i << ", "; }
  cout << endl;
  cout << "Number of nodes: " << g.node_count() << endl;
  cout << "Status of node 0: " << g.get_node_status(0) << endl;
  
  return 0;
}


