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

void construct_csr(vector<unique_ptr<vector<int>>>& adjacencies,
                   vector<int>& offsets, vector<int>& neighbors) {
  int offset = 0;
  for (auto& adjacency : adjacencies) {
    offsets.push_back(offset);
    offset += adjacency->size();
    for (auto& neighbor : *adjacency) { neighbors.push_back(neighbor); }
  }
}

void Graph::input_from_file(string file_name) {
  vector<int> offsets;
  vector<int> neighbors;
  fstream newfile;
  newfile.open(file_name, ios::in);
  if (newfile.is_open()) {
    string tp;
    while(getline(newfile, tp)) {
      offsets.push_back(neighbors.size());
      vector<string> vctr = split(tp, " ");
      bool first = false;
      for (auto j : vctr) {
        if (!first) {
          first = true;
        } else {
          neighbors.push_back(stoi(j));
        }
      }
    }
    newfile.close();
  }
  append_region(offsets, neighbors);
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
  vector<int> offsets;
  vector<int> neighbors;
  int N = L * L;
  for (int j = 0; j < N; ++j) {
    int agent_x = get_x(j, L);
    int agent_y = get_y(j, L);
    int min_x = max(agent_x - Dt, 0);
    int min_y = max(agent_y - Dt, 0);
    int max_x = min(agent_x + Dt + 1, L);
    int max_y = min(agent_y + Dt + 1, L);
    offsets.push_back(neighbors.size());
    for (int y = min_y; y < max_y; ++y) {
      for (int x = min_x; x < max_x; ++x) {
        int index = get_index(x, y, L);
        if (index != j) {
          neighbors.push_back(get_index(x, y, L));
        }
      }
    }
  }
  append_region(offsets, neighbors);
}

void Graph::nw_small_world(int N, int k, float p) {
  vector<unique_ptr<vector<int>>> adjacencies;

  for (int i = 0; i < N; i++) {
    adjacencies.push_back(make_unique<vector<int>>());
    for (int j = 1; j <= k; j++) {
      adjacencies[i]->push_back((i + j) % N);
      adjacencies[i]->push_back((i + N - j) % N);
    }
  }

  for (int i = 0; i < N; i++) {
    int possible = max(0, N - (i + k + 1));
    int n_connections = sample_binomial(possible, p);
    set<int> connections = unique_random_numbers(n_connections, possible);
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

void Graph::random_graph(int N, int N0) {
  clog << endl << " N = " << N << " , N0 = " << N0
       << " , Nchk1 = " << Nchk1 << " , Nchk2 = " << Nchk2 << endl;
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


/*
vector<int> init_offsets(int N, int N0, int nr_existing_connections) {
  vector<int> offsets2add(N,nr_existing_connections);
  for (int j = 1; j < (int)offsets2add.size(); j++)
    { offsets2add[j] = offsets2add[j-1] + N0; }
  return offsets2add;
}

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
