#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <string>
#include <vector>
#include "Agent.hpp"
#include "Graph.hpp"
#include "Group.hpp"
#include "Utils.hpp"

template<typename T>
string to_string(set<T> set_) {
  string to_string = "< ";
  for(T elem : set_) { to_string += to_string(elem) + " "; }
  to_string += ">";
  return to_string;
}

template<typename T>
string to_string(vector<T> vctr) {
  string to_string = "[ ";
  for(T elem : vctr) { to_string += std::to_string(elem) + " "; }
  to_string += "]";
  return to_string;
}

string to_string(vector<Agent> vctr) {
  string to_string = "[ ";
  for(Agent agent : vctr) { to_string += agent.to_string() + " "; }
  to_string += "]";
  return to_string;
}

void Graph::print_agents_edges_offsets(string model) {
  clog << endl << model << endl
    << " agents = " << to_string(agents) << endl
    << " neighbrs = " << to_string(neighbrs) << endl
    << " offsets = " << to_string(offsets) << endl
    << " agents.size() = " << std::to_string(agents.size()) << endl
    << " neighbrs.size() = " << std::to_string(neighbrs.size()) << endl
    << " offsets.size() = " << std::to_string(offsets.size()) << endl;
}

void Graph::append_region(vector<int> new_offsets, vector<int> new_neighbors) {
  int n_existing_neighbors = neighbrs.size();
  int n_existing_agents = agents.size();
  int id = n_existing_agents;
  
  region_agent_offsets.push_back(n_existing_agents);  
  for (auto& offset : new_offsets) {
    offsets.push_back(offset + n_existing_neighbors);
    agents.push_back(Agent(id++));
  }
  for (auto& neighbor : new_neighbors)
    neighbrs.push_back(neighbor + n_existing_agents);
}

void Graph::assign_groups(vector<shared_ptr<Group>>& groups) {
  if (groups.size() > 0) {
    for (Agent& agent : agents) {
      int j = sample_nonnegative(groups.size());
      agent.assign_group(groups[j]);
    }
  }
}

vector<int> Graph::neighbours(int id) {
  vector<int> neighbours;
  int start = offsets[id];
  int end;
  if (id + 1 < (int)offsets.size()) {
    end = offsets[id + 1];
  } else {
    end = neighbrs.size();
  }


  for (int j = start; j < end;  j++) {
    neighbours.push_back(neighbrs[j]);
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

vector<int> Graph::get_neighboring_regions(int region_id) {
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
  write_vector_to_stream_line(stream, neighbrs, false);
}

void Graph::read_generatable_graph(istream& stream) {
  int id = 0;
  int nr_existing_agents = agents.size();
  int nr_existing_connections = neighbrs.size();
  string line;

  // Add the region offsets
  getline(stream, line);
  vector<string> entries = split(line, " ");
  for (auto& entry : entries)
    region_agent_offsets.push_back(stoi(entry) + nr_existing_agents);

  // Add the offsets
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries) {
    offsets.push_back(stoi(entry) + nr_existing_connections);
    agents.push_back(Agent(id++ + nr_existing_agents));
  }

  // Add the neighbrs
  getline(stream, line);
  entries = split(line, " ");
  for (auto& entry : entries)
    neighbrs.push_back(stoi(entry) + nr_existing_agents);
}

int Graph::start_new_region() {
  int nr_of_previous_agents = agents.size();
  region_agent_offsets.push_back(nr_of_previous_agents);
  return nr_of_previous_agents = agents.size();
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
