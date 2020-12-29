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

void Graph::input_from_file(string file_name) {
  start_new_region();
  fstream newfile;
  newfile.open(file_name, ios::in);
  if (newfile.is_open()) {
    string tp;
    while(getline(newfile, tp)) {

      offsets.push_back(edges.size());

      vector<string> v = split(tp, " ");
      bool first = false;
      for (auto j : v) {
        if (!first) {
          first = true;
          agents.push_back(Agent(stoi(j)));
        } else {
          edges.push_back(stoi(j));
        }
      }
    }
    newfile.close();
  }
}

int get_x(int j, int matrix_size) {
  return j % matrix_size;
}

int get_y(int j, int matrix_size) {
  return j / matrix_size;
}

int get_index(int x, int y, int matrix_size) {
  return (matrix_size * y) + x;
}

void Graph::matrix_graph(int L, int Dt) {
  start_new_region();
  int n_existing_agents = agents.size();
  for (int j = 0; j < (L * L); ++j) {

    int agent_x = get_x(j, L);
    int agent_y = get_y(j, L);

    int min_x = max(agent_x - Dt, 0);
    int min_y = max(agent_y - Dt, 0);

    int max_x = min(agent_x + Dt + 1, L);
    int max_y = min(agent_y + Dt + 1, L);

    offsets.push_back(edges.size());
    agents.push_back(Agent(j + n_existing_agents));

    for (int y = min_y; y < max_y; ++y) {
      for (int x = min_x; x < max_x; ++x) {
        int index = get_index(x, y, L);
        if (index != j) {
          edges.push_back(get_index(x, y, L) + n_existing_agents);
        }
      }
    }
  }
}


bool roll(float p) {
  return p > (float) rand() / (float) RAND_MAX;
}

void Graph::nw_small_world(int N, int k, float p) {
  start_new_region();

  // Vector for storing connection vectors
  vector<unique_ptr<vector<int>>> adjacencies;

  // Create the initial adjacencies
  for (int i = 0; i < N; i++) {
    adjacencies.push_back(make_unique<vector<int>>());
    for (int j = 1; j <= k; j++) {
      adjacencies[i]->push_back((i + j) % N);
      adjacencies[i]->push_back((i + N - j) % N);
    }
  }

  // Create random adjacencies
  for (int i = 0; i < N; i++) {
    for (int j = i + k + 1; j < N; j++) {
      if (j < i + N - k && roll(p)) {
        adjacencies[i]->push_back(j);
        adjacencies[j]->push_back(i);
      }
    }
  }

  // Sort each adjacency list
#pragma omp parallel for
  for (int j = 0; j < (int) adjacencies.size(); j++)
    sort(adjacencies[j]->begin(), adjacencies[j]->end());

  // Store all adjacencies in the compressed row vectors
  int id = 0;
  int n_existing_agents = agents.size();
  int n_existing_connections = edges.size();
  for (auto& adjacency : adjacencies) {
    agents.push_back(Agent(id++ + n_existing_agents));
    offsets.push_back(n_existing_connections);
    n_existing_connections += adjacency->size();
    for (auto edge : *adjacency)
      edges.push_back(edge + n_existing_agents);
  }
}


void Graph::default_graph() {
  matrix_graph(20, 1);
}

void Graph::assign_groups(vector<shared_ptr<Group>>& groups) {
  if (groups.size() > 0) {
    for (Agent& agent : agents) {
      int j = rand() % groups.size();
      agent.assign_group(groups[j]);
    }
  }
}

// Return neighbours of agent id:
vector<int> Graph::neighbours(int id) {
  vector<int> neighbours;
  int start = offsets[id];
  int end;
  if (id + 1 < (int)offsets.size()) {
    end = offsets[id + 1];
  } else {
    end = edges.size();
  }


  for (int j = start; j < end;  j++) {
    neighbours.push_back(edges[j]);
  }

  return neighbours;
}

int Graph::agents_count() {
  return offsets.size();
}

Agent Graph::get_agent(int id) {
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
  for (int j = region_start; j < region_end; j++)
    neighbouring_regions.push_back(region_connections[j]);

  return neighbouring_regions;
}

void Graph::print_graph() {
  clog << "Offsets: ";
  for (auto j : offsets) { clog << j << ", "; }
  clog << endl;
  clog << "Edges: ";
  for (auto j : edges) { clog << j << ", "; }
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
  write_vector_to_stream_line(stream, offsets, true);
  write_vector_to_stream_line(stream, edges, false);
}

void Graph::read_generatable_graph(istream& stream) {
  int id = 0;
  int n_existing_agents = agents.size();
  int n_existing_connections = edges.size();
  string line;

  // Add the region offsets
  getline(stream, line);
  vector<string> entries = split(line, " ");
  for (auto& entry : entries)
    region_agent_offsets.push_back(stoi(entry) + n_existing_agents);

  // Add the offsets
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries) {
    offsets.push_back(stoi(entry) + n_existing_connections);
    agents.push_back(Agent(id++ + n_existing_agents));
  }

  // Add the edges
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries)
    edges.push_back(stoi(entry) + n_existing_agents);
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
