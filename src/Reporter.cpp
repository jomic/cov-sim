#include <vector>
#include <utility>
#include <iostream>
#include "../include/json.hpp"
#include "Reporter.hpp"

using namespace std;
using json = nlohmann::json;

vector<vector<pair<int, int>>> reports;
vector<vector<int>> connections;

void start_new_report() {
  vector<pair<int, int>> new_report;
  reports.push_back(new_report);
}

void report(int id, int state) {
  // Let's just assume we start a new report when 0 reports
  if (id == 0)
    start_new_report();

  reports[reports.size() - 1].push_back(pair<int, int>(id, state));
}

void report_connections(int id, vector<int> neighbors) {
  connections.push_back(neighbors);
}

void finish_report() {
  json output = {
    {"nodes_timeline", {}},
    {"links", {}}
  };
  for (auto& report : reports) {
    json nodes = {
      {"nodes", {}}
    };
    for (auto& node : report) {
      json added = {
	{"id", node.first},
	{"state", node.second}
      };
      nodes["nodes"].push_back(added);
    }
    output["nodes_timeline"].push_back(nodes);
  }

  int id = 0;
  for (auto& connection : connections) {
    for (auto& target : connection) {
      json link = {
	{"source", id},
	{"target", target},
	{"value", 1}
      };
      output["links"].push_back(link);
    }
    id++;
  }
  
  cout << output;
}
