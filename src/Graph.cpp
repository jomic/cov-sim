#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Agent.hpp"
#include "Graph.hpp"
#include "Group.hpp"
#include "Utils.hpp"
using namespace std;

Relation::Relation(int d) : distance(d) {}

void Graph::input_from_file(string file_name) {
  start_new_region();
  fstream newfile;
  newfile.open(file_name, ios::in);
  if (newfile.is_open()) {
    string tp;
    while(getline(newfile, tp)) {

      offsetsVctr.push_back(edgesVctr.size());

      vector<string> v = split(tp, " ");
      bool first = false;
      for (auto i : v) {
        if (!first) {
          first = true;
          agents.push_back(Agent(stoi(i)));
        } else {
          edgesVctr.push_back(stoi(i));
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
  start_new_region();
  int n_existing_agents = agents.size();
  for (int i = 0; i < (n * n); ++i) {

    int node_x = get_x(i, n);
    int node_y = get_y(i, n);

    int min_x = max(node_x - d, 0);
    int min_y = max(node_y - d, 0);

    int max_x = min(node_x + d + 1, n);
    int max_y = min(node_y + d + 1, n);

    offsetsVctr.push_back(edgesVctr.size());
    agents.push_back(Agent(i + n_existing_agents));

    for (int y = min_y; y < max_y; ++y) {
      for (int x = min_x; x < max_x; ++x) {
        int index = get_index(x, y, n);
        if (index != i) {
          edgesVctr.push_back(get_index(x, y, n) + n_existing_agents);
        }
      }
    }
  }
}


bool roll(float p) {
  return p > (float) rand() / (float) RAND_MAX;
}

void Graph::nw_small_world(int l, int k, float p) {
  start_new_region();

  // Vector for storing connection vectors
  vector<unique_ptr<vector<int>>> adjacencies;

  // Create the initial adjacencies
  for (int i = 0; i < l; i++) {
    adjacencies.push_back(make_unique<vector<int>>());
    for (int j = 1; j <= k; j++) {
      adjacencies[i]->push_back((i + j) % l);
      adjacencies[i]->push_back((i + l - j) % l);
    }
  }

  // Create random adjacencies
  for (int i = 0; i < l; i++) {
    for (int j = i + k + 1; j < l; j++) {
      if (j < i + l - k && roll(p)) {
        adjacencies[i]->push_back(j);
        adjacencies[j]->push_back(i);
      }
    }
  }

  // Sort each adjacency list
#pragma omp parallel for
  for (int i = 0; i < (int) adjacencies.size(); i++)
    sort(adjacencies[i]->begin(), adjacencies[i]->end());

  // Store all adjacencies in the compressed row vectors
  int id = 0;
  int n_existing_agents = agents.size();
  int n_existing_connections = edgesVctr.size();
  for (auto& adjacency : adjacencies) {
    agents.push_back(Agent(id++ + n_existing_agents));
    offsetsVctr.push_back(n_existing_connections);
    n_existing_connections += adjacency->size();
    for (auto edge : *adjacency)
      edgesVctr.push_back(edge + n_existing_agents);
  }
}


void Graph::default_graph() {
  matrix_graph(20, 1);
}

void Graph::assign_groups(vector<shared_ptr<group_t>>& groups) {
  if (groups.size() > 0) {
    for (Agent& agent : agents) {
      int i = rand() % groups.size();
      agent.assign_group(groups[i]);
    }
  }
}

// Return neighbours of node id:
vector<int> Graph::neighbours(int id) {
  vector<int> neighbours;
  int start = offsetsVctr[id];
  int end;
  if (id + 1 < (int)offsetsVctr.size()) {
    end = offsetsVctr[id + 1];
  } else {
    end = edgesVctr.size();
  }


  for (int i = start; i < end;  i++) {
    neighbours.push_back(edgesVctr[i]);
  }

  return neighbours;
}

int Graph::node_count() {
  return offsetsVctr.size();
}

Agent Graph::get_node(int id) {
  return agents[id];
}

int Graph::get_agent_region(int id) {
  int region = -1;
  for (auto& offset : region_agent_offsets) {
    if (id >= offset)
      region++;
    else
      break;
  }
  return region;
}

vector<int> Graph::get_neighbouring_regions(int region_id) {
  int region_start = region_connection_offsets[region_id];
  int region_end;
  if (region_id == (int) region_connection_offsets.size() - 1)
    region_end = region_connections.size();
  else
    region_end = region_connection_offsets[region_id + 1];

  vector<int> neighbouring_regions;
  for (int i = region_start; i < region_end; i++)
    neighbouring_regions.push_back(region_connections[i]);

  return neighbouring_regions;
}

void Graph::print_graph() {
  clog << "Offsets: ";
  for (auto i : offsetsVctr) { clog << i << ", "; }
  clog << endl;
  clog << "Edges: ";
  for (auto i : edgesVctr) { clog << i << ", "; }
  clog << endl;
}

void write_vector_to_stream_line(
      ostream& stream, vector<int>& vec, bool newline) {
  bool first = true;
  for (auto& entry : vec) {
    if (first)
      first = false;
    else
      stream << " ";
    stream << entry;
  }
  if (newline)
    stream << endl;
}

void Graph::write_generatable_graph(ostream& stream) {
  write_vector_to_stream_line(stream, region_agent_offsets, true);
  write_vector_to_stream_line(stream, offsetsVctr, true);
  write_vector_to_stream_line(stream, edgesVctr, false);
}

void Graph::read_generatable_graph(istream& stream) {
  int id = 0;
  int n_existing_agents = agents.size();
  int n_existing_connections = edgesVctr.size();
  string line;

  // Add the region offsetsVctr
  getline(stream, line);
  vector<string> entries = split(line, " ");
  for (auto& entry : entries)
    region_agent_offsets.push_back(stoi(entry) + n_existing_agents);

  // Add the offsetsVctr
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries) {
    offsetsVctr.push_back(stoi(entry) + n_existing_connections);
    agents.push_back(Agent(id++ + n_existing_agents));
  }

  // Add the edgesVctr
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries)
    edgesVctr.push_back(stoi(entry) + n_existing_agents);
}

void Graph::start_new_region() {
  int n_agents = agents.size();
  region_agent_offsets.push_back(n_agents);
}

void Graph::set_region_connections(istream& stream) {
  string line;
  vector<string> entries;

  while (getline(stream, line)) {
    entries = split(line, " ");
    region_connection_offsets.push_back(region_connections.size());
    for (auto& entry : entries)
      region_connections.push_back(stoi(entry));
  }

  if (region_connection_offsets.size() != region_agent_offsets.size())
    cerr << "WARNING -- Inconsistent region count." << endl;
}

void Graph::set_region_connections(vector<vector<int>>& connections) {
  for (auto connection_list : connections) {
    region_connection_offsets.push_back(region_connections.size());
    for (auto connection : connection_list)
      region_connections.push_back(connection);
  }

  if (region_connection_offsets.size() != region_agent_offsets.size())
    cerr << "WARNING -- Inconsistent region count." << endl;
}

void Graph::default_region_connections() {
  for (auto offset : region_agent_offsets) {
    (void)offset;
    region_connection_offsets.push_back(0);
  }
}
