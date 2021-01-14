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

string to_string(vector<Agent> vctr) {
  string to_string = "[ ";
  for(Agent agent : vctr) { to_string += agent.to_string() + " "; }
  to_string += "]";
  return to_string;
}

template<typename T>
string to_string(vector<T> vctr) {
  string to_string = "[ ";
  for(T elem : vctr) { to_string += std::to_string(elem) + " "; }
  to_string += "]";
  return to_string;
}

void construct_csr(vector<unique_ptr<vector<int>>>& adjacencies,
		   vector<int>& offsets,
		   vector<int>& neighbors) {
  int offset = 0;
  for (auto& adjacency : adjacencies) {
    offsets.push_back(offset);
    offset += adjacency->size();
    for (auto& neighbor : *adjacency)
      neighbors.push_back(neighbor);
  }
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

void Graph::input_from_file(string file_name) {
  start_new_region();
  fstream newfile;
  newfile.open(file_name, ios::in);
  if (newfile.is_open()) {
    string tp;
    while(getline(newfile, tp)) {
      offsets.push_back(neighbrs.size());
      vector<string> vctr = split(tp, " ");
      bool first = false;
      for (auto j : vctr) {
        if (!first) {
          first = true;
          agents.push_back(Agent(stoi(j)));
        } else {
          neighbrs.push_back(stoi(j));
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
  int nr_existing_agents = start_new_region(), N = L * L;
  for(int j=0; j<N; j++) { agents.push_back(Agent(j + nr_existing_agents)); }
  for (int j = 0; j < N; ++j) {
    int agent_x = get_x(j, L);
    int agent_y = get_y(j, L);
    int min_x = max(agent_x - Dt, 0);
    int min_y = max(agent_y - Dt, 0);
    int max_x = min(agent_x + Dt + 1, L);
    int max_y = min(agent_y + Dt + 1, L);
    offsets.push_back(neighbrs.size());
    for (int y = min_y; y < max_y; ++y) {
      for (int x = min_x; x < max_x; ++x) {
        int index = get_index(x, y, L);
        if (index != j) {
          neighbrs.push_back(get_index(x, y, L) + nr_existing_agents);
        }
      }
    }
  }
}

bool roll(float p) {
  return p > (float) rand() / (float) RAND_MAX;
}

void Graph::nw_small_world(int N, int k, float p) {
  vector<unique_ptr<vector<int>>> adjacencies;

  // Create the initial adjacencies
  for (int i = 0; i < N; i++) {
    adjacencies.push_back(make_unique<vector<int>>());
    for (int j = 1; j <= k; j++) {
      adjacencies[i]->push_back((i + j) % N);
      adjacencies[i]->push_back((i + N - j) % N);
    }
  }

  // Randomize the number of connections
  std::default_random_engine generator;
  for (int i = 0; i < N; i++) {
    int possible = std::max(0, N - (i + k + 1));
    std::binomial_distribution<int> distribution(possible, p);
    int n_connections = distribution(generator);
    std::set<int> connections = unique_random_numbers(n_connections, possible);
    for (auto c : connections) {
      int target = c + i + k + 1;
      adjacencies[i]->push_back(target);
      adjacencies[target]->push_back(i);
    }
  }

  vector<int> offsets;
  vector<int> neighbors;
  construct_csr(adjacencies, offsets, neighbors);
  append_region(offsets, neighbors);
}

vector<int> init_offsets(int N, int N0, int nr_existing_connections) {
  vector<int> offsets2add(N,nr_existing_connections);
  for (int j = 1; j < (int)offsets2add.size(); j++)
    { offsets2add[j] = offsets2add[j-1] + N0; }
  return offsets2add;
}

int sum(vector<int> vctr) {
  int sum = 0;
  for(int i : vctr) { sum += i; }
  return sum;
}

template<typename T>
string to_string(set<T> set_) {
  string to_string = "< ";
  for(T elem : set_) { to_string += std::to_string(elem) + " "; }
  to_string += ">";
  return to_string;
}

/*
void Graph::random_graph(int N, int N0) {
  int nr_existing_agents = start_new_region();
  vector<Agent> agents2add(N);
  for(int j=0; j<N; j++) { agents2add[j]=Agent(j+nr_existing_agents); }
  agents.insert(agents.end(), agents2add.begin(), agents2add.end());
  int Ne=N*N0, candidateEdge, Nchk1=14*sqrt(sqrt(N));
  if (N<50) { Nchk1=4*N/5-4; }
  int Nchk2=Nchk1 + 2*N/N0;
  clog << endl << " N = " << N << " , N0 = " << N0
       << " , Nchk1 = " << Nchk1 << " , Nchk2 = " << Nchk2 << endl;
  if (!(Nchk1 < N)) {
    cerr << endl << "!! Each node must have (a lot) fewer edges than"
         << " there are nodes in the graph !" << endl << endl;
    return;
  }
  vector<int> offsets2add = init_offsets(N,N0,neighbrs.size()), belowN0(N),
      nebrs2ad(Ne);
  vector<set<int>> nodeEdges(N);
  for (int j = N-1; j > Nchk2; j--) {
    while ((int)nodeEdges[j].size() < N0) {
      candidateEdge = rand() % j;
      if ((int)nodeEdges[candidateEdge].size() < N0) {
        nodeEdges[candidateEdge].insert(j);
        nodeEdges[j].insert(candidateEdge);
      }
    }
  }
  for (int j = Nchk2; j > Nchk1; j--) {
    int k = 0;
    while ((int)nodeEdges[j].size() < N0) {
      candidateEdge = rand() % j;
      if ((int)nodeEdges[candidateEdge].size() < N0) {
        nodeEdges[candidateEdge].insert(j);
        nodeEdges[j].insert(candidateEdge);
      }
      k++;
      if (k > N) {
        cerr << endl << "  - - !! ERROR ! -"
          << " Stuck in infinite loop !! - -" << endl << endl;
        return;
      }
    }
    }
  for (int k = N-1; k > -1; k--) {
    int l = 0;
    for(int edg : nodeEdges[k]) { nebrs2ad[k*N0 + l] = edg; l++; }
  }
  for (int j = Nchk1; j > -1; j--) {
    set<int> candidatEdges = nodeEdges[j];
    vector<int> nonFullNodes(j);
    for (int k = 0; k < j; k++) {
      if ((int)nodeEdges[k].size() < N0) {
        nonFullNodes[k] = 1;
        candidatEdges.insert(k);
      }
    }
    if ((int)nodeEdges[j].size() + sum(nonFullNodes) > N0) {
      while ((int)nodeEdges[j].size() < N0) {
        candidateEdge = rand() % j;
        if ((int)nodeEdges[candidateEdge].size() < N0) {
          nodeEdges[candidateEdge].insert(j);
          nodeEdges[j].insert(candidateEdge);
        }
      }
    } else if ((int)nodeEdges[j].size() + sum(nonFullNodes) == N0) {
      nodeEdges[j] = candidatEdges;
    } else if ((int)nodeEdges[j].size() + sum(nonFullNodes) < N0) {
      nodeEdges[j] = candidatEdges;
      belowN0[j] = N0 - candidatEdges.size();
      nebrs2ad.erase(
          nebrs2ad.begin()+(j+1)*N0-belowN0[j],nebrs2ad.begin()+(j+1)*N0);
      for (int l = j+1; l < N; l++) { offsets2add[l] -= belowN0[j]; }
    } else {
      cerr << endl << "= = = = = = = = = = = = = = = = = = = ="
           << endl << "! Programmer'§ ERROR ! - Go complain!!"
           << endl << "= = = = = = = = = = = = = = = = = = = =" << endl <<endl;
      return;
    } // Below, candidate edges in nodeEdges[j] with (cdt < j) are approved:
    for(int cdt : nodeEdges[j]) { if (cdt<j) { nodeEdges[cdt].insert(j); } }
    for (int k = N-1; k > -1; k--) {
      int l = 0;
      for(int edg : nodeEdges[k]) { nebrs2ad[k*N0 + l] = edg; l++; }
    }
  }
// Below, `nebrs2ad` is _completely_ recreated from all the `nodeEdges[k]` sets.
// The only 'inheritance' is from line 108 which reduces the size of `nebrs2ad`.
// Also, each edge must be adjusted for previous regions (+nr_existing_agents).
  int m = 0;
  for (int k = 0; k < N; k++) {
    for(int edg : nodeEdges[k]){ nebrs2ad[m] = edg+nr_existing_agents; m++; }
  }
  neighbrs.insert(neighbrs.end(), nebrs2ad.begin(), nebrs2ad.end());
  offsets.insert(offsets.end(), offsets2add.begin(), offsets2add.end());
}
*/


void Graph::random_graph(int N, int N0) {
  vector<unique_ptr<vector<int>>> adjacencies;
  vector<int> unfilled;
  vector<int> degrees;
  for (int i = 0; i < N; i++) {
    adjacencies.push_back(make_unique<vector<int>>());
    unfilled.push_back(i);
    degrees.push_back(0);
  }

  int source_id;
  while (unfilled.size() > 1) {
    source_id = unfilled.back();
    unfilled.pop_back();
    int n_needed = N0 - degrees[source_id];
    int n_available = unfilled.size();
    int n_added = min(n_needed, n_available);
    set<int> targets_set = unique_random_numbers(n_added, n_available);
    vector<int> targets(targets_set.rbegin(), targets_set.rend());

    for (auto& target : targets) {
      int target_id = unfilled[target];
      adjacencies[source_id]->push_back(target_id);
      adjacencies[target_id]->push_back(source_id);
      degrees[source_id]++;
      if (++degrees[target_id] >= N0)
	unfilled.erase(unfilled.begin() + target);
    }
  }

  vector<int> offsets;
  vector<int> neighbors;
  construct_csr(adjacencies, offsets, neighbors);
  append_region(offsets, neighbors);
}

void Graph::default_graph() {
  random_graph(6,3);
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
