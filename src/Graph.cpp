#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include "Agent.hpp"
#include "Graph.hpp"
#include "Group.hpp"
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
          node_values.push_back(Agent(stoi(i)));
        } else {
          edges.push_back(stoi(i));
            
        }
      }
    }
    newfile.close();
  }
}

int get_x(int i, int matrix_size) {
  return i % matrix_size;
}

int get_y(int i, int matrix_size) {
  return i / matrix_size;
}

int get_index(int x, int y, int matrix_size) {
  return (matrix_size * y) + x;
}

void Graph::matrix_graph(int n, int d) {

  for (int i = 0; i < (n * n); ++i) {
    
    int node_x = get_x(i, n);
    int node_y = get_y(i, n);

    int min_x = max(node_x - d, 0);
    int min_y = max(node_y - d, 0);

    int max_x = min(node_x + d + 1, n);
    int max_y = min(node_y + d + 1, n);

    offsets.push_back(edges.size());
    node_values.push_back(Agent(i));
        
    for (int y = min_y; y < max_y; ++y) {
      for (int x = min_x; x < max_x; ++x) {
        int index = get_index(x, y, n);
        if (index != i) {
          edges.push_back(get_index(x, y, n));
        }
      }
    }
  }
}

void Graph::default_graph() {
  matrix_graph(20, 1);
}

void Graph::assign_groups(vector<shared_ptr<group_t>>& groups) {
  if (groups.size() > 0) {
    for (Agent& agent : node_values) {
      int i = rand() % groups.size();
      agent.assign_group(groups[i]);
    }
  }
}

// Return neighbours of node id:
vector<int> Graph::neighbours(int id) {
  vector<int> neighbours;
  int start = offsets[id];
  int end;
  if (id + 1 < (int)offsets.size()) {
    end = offsets[id + 1];
  } else {
    end = edges.size();
  }


  for (int i = start; i < end;  i++) {
    neighbours.push_back(edges[i]);
  }

  return neighbours;
}

int Graph::node_count() {
  return offsets.size();
}

Agent Graph::get_node(int id) {
  return node_values[id];
}

void Graph::print_graph() {
  clog << "Offsets: ";
  for (auto i : offsets) { clog << i << ", "; }
  clog << endl;
  clog << "Edges: ";
  for (auto i : edges) { clog << i << ", "; }
  clog << endl;
}

void Graph::write_generatable_graph(ostream& stream) {
  // Write the region offsets
  bool first = true;
  for (auto region_offset : region_agent_offsets) {
    if (first)
      first = false;
    else stream << " ";
    stream << region_offset;
  }
  stream << endl;

  // Write the offsets
  first = true;
  for (auto offset : offsets) {
    if (first)
      first = false;
    else
      stream << " ";
    stream << offset;
  }
  stream << endl;

  // Write the edges
  first = true;
  for (auto edge : edges) {
    if (first)
      first = false;
    else
      stream << " ";
    stream << edge;
  }
}

void Graph::read_generatable_graph(istream& stream) {
  int id = 0;
  int id_offset = node_values.size();
  int offset = edges.size();
  string line;

  // Add the region offsets
  getline(stream, line);
  vector<string> entries = split(line, " ");
  for (auto& entry : entries)
    region_agent_offsets.push_back(stoi(entry) + id_offset);

  // Add the offsets
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries) {
    offsets.push_back(stoi(entry) + offset);
    node_values.push_back(Agent(id++ + id_offset));
  }

  // Add the edges
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries)
    edges.push_back(stoi(entry) + id_offset);
}
