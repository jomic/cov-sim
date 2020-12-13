#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include "Graph.hpp"
#include "Agent.hpp"
#include "omp.h"
using namespace std;

bool roll(float p) {
  return p > (float) rand() / (float) RAND_MAX;
}

void Graph::nw_small_world(int l, int k, float p) {
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
  for (int i = 0; i < adjacencies.size(); i++)
    sort(adjacencies[i]->begin(), adjacencies[i]->end());

  // Store all adjacencies in the compressed row vectors
  int id = 0;
  int n_existing_agents = node_values.size();
  int n_existing_connections = edges.size();
  region_agent_offsets.push_back(n_existing_agents);
  for (auto& adjacency : adjacencies) {
    node_values.push_back(Agent(id++ + n_existing_agents));
    offsets.push_back(n_existing_connections);
    n_existing_connections += adjacency->size();
    for (auto edge : *adjacency)
      edges.push_back(edge + n_existing_agents);
  }
}
