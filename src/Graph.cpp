#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
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

int sum(vector<int> v) {
  int sum = 0;
  for(int i : v) { sum += i; }
  return sum;
}

template<typename T>
string to_string(vector<T> v) {
  string to_string = "[ ";
  for(T e : v) { to_string += std::to_string(e) + " "; }
  to_string += "]";
  return to_string;
}

template<typename T>
string to_string(set<T> s) {
  string to_string = "< ";
  for(T e : s) { to_string += std::to_string(e) + " "; }
  to_string += ">";
  return to_string;
}

vector<int> init_offsets(vector<int> v, int N0) {
  v[0] = 0;
  for (int i = 1; i < (int)v.size(); i++) {
    v[i] = v[i-1] + N0;
  }
  return v;
}

void Graph::random_graph(int N, int N0) {
  int Ne=N*N0, candidateEdge, Nchk1=13.8*sqrt(sqrt(N)), Nchk2=Nchk1+2*N/N0;
  cout << endl << " N = " << N << ", N0 = " << N0
       << ", Nchk1 = " << Nchk1 << ", Nchk2 = " << Nchk2 << endl;
  if (!(Nchk1 < N)) {
    cout << endl << "!! Each node must have (a lot) fewer edges than"
         << " there are nodes in the graph !" << endl << endl;
    return;
  }
  vector<int> edges(Ne, -9), offsets(N), belowN0(N);
  offsets = init_offsets(offsets, N0);
  // cout << endl << " At start, directly after declarations:" << endl;
  // cout << " edges = " << to_string(edges) << endl;
  // cout << " offsets = " << to_string(offsets) << endl;
  // cout << " edges.size() = " << std::to_string(edges.size()) << endl;
  vector<set<int>> nodeEdges(N);
  srand(time(NULL));
  for (int i = N-1; i > Nchk2; i--) {
    // cout << " First for-loop " << endl;
    while ((int)nodeEdges[i].size() < N0) {
      candidateEdge = rand() % i;
      if ((int)nodeEdges[candidateEdge].size() < N0) {
        nodeEdges[candidateEdge].insert(i);
        nodeEdges[i].insert(candidateEdge);
      }
    }
  }
  for (int i = Nchk2; i > Nchk1; i--) {
    // cout << " Second for-loop " << endl;
    int j = 0;
    while ((int)nodeEdges[i].size() < N0) {
      candidateEdge = rand() % i;
      if ((int)nodeEdges[candidateEdge].size() < N0) {
        nodeEdges[candidateEdge].insert(i);
        nodeEdges[i].insert(candidateEdge);
      }
      j++;
      if (j > N) {
        cout << endl << "  - - !! ERROR ! - -" << endl;
        cout << endl << " !!! Stuck in infinite loop !!" << endl << endl;
        return;
      }
    }
    }
  for (int j = N-1; j > -1; j--) {
    int k = 0;
    for(int e : nodeEdges[j]) { edges[j*N0 + k] = e; k++; }
  }
  // cout << endl << " i > Nchk1:" << endl;
  // cout << " edges = " << to_string(edges) << endl;
  // cout << " offsets = " << to_string(offsets) << endl;
  // cout << " edges.size() = " << std::to_string(edges.size()) << endl;
  for (int i = Nchk1; i > -1; i--) {
    set<int> candidatEdges = nodeEdges[i];
    // cout << endl << " i = " << i;
    // cout << ", BEFORE: candidatEdges = " << to_string(candidatEdges);
    vector<int> nonFullNodes(i);
    for (int j = 0; j < i; j++) {
      if ((int)nodeEdges[j].size() < N0) {
        nonFullNodes[j] = 1;
        candidatEdges.insert(j);
      }
    }
    // cout << endl << " AFTER: candidatEdges = " << to_string(candidatEdges);
    // cout << ", nodeEdges[" << i << "].size() = "
        // << std::to_string(nodeEdges[i].size())
        // << ", sum(nonFullNodes) = " << sum(nonFullNodes) << endl;
    if ((int)nodeEdges[i].size() + sum(nonFullNodes) > N0) {
      while ((int)nodeEdges[i].size() < N0) {
        candidateEdge = rand() % i;
        if ((int)nodeEdges[candidateEdge].size() < N0) {
          nodeEdges[candidateEdge].insert(i);
          nodeEdges[i].insert(candidateEdge);
        }
      }
    } else if ((int)nodeEdges[i].size() + sum(nonFullNodes) == N0) {
      nodeEdges[i] = candidatEdges;
    } else if ((int)nodeEdges[i].size() + sum(nonFullNodes) < N0) {
      nodeEdges[i] = candidatEdges;
      belowN0[i] = N0 - candidatEdges.size();
      edges.erase(edges.begin()+(i+1)*N0-belowN0[i],edges.begin()+(i+1)*N0);
      for (int k = i+1; k < N; k++) { offsets[k] -= belowN0[i]; }
    } else {
      cout << endl << "= = = = = = = = = = = = = = = = = = = ="
           << endl << "! Programmer's ERROR ! - Go complain!!"
           << endl << "= = = = = = = = = = = = = = = = = = = =" << endl <<endl;
      return;
    }
    for(int c : nodeEdges[i]) { if (c < i) { nodeEdges[c].insert(i); } }
    for (int j = N-1; j > -1; j--) {
      int k = 0;
      for(int e : nodeEdges[j]) { edges[j*N0 + k] = e; k++; }
    } // The line below outputs the WRONG `edges`, but this is just TEMPORARY:
    // cout << " edges (WRONG) = " << to_string(edges) << endl;
    // cout << " offsets = " << to_string(offsets) << endl;
    // cout << " edges.size() = " << std::to_string(edges.size()) << endl;
  }
// // Below, `edges` is _completely_ recreated from all the `nodeEdges[j]` sets.
// // The only 'inheritance' is from line 108 which reduces the size of `edges`.
  int l = 0;
  for (int j = 0; j < N; j++) {
    for(int e : nodeEdges[j]) { edges[l] = e; l++; }
  }
  cout << endl << " Finally:" << endl;
  // cout << " edges = " << to_string(edges) << endl;
  // cout << " offsets = " << to_string(offsets) << endl;
  cout << " edges.size() = " << std::to_string(edges.size()) << endl;
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
