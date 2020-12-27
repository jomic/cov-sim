#include <fstream>
#include <iostream>
#include "../include/json.hpp"
#include <memory>
#include <vector>
#include "Graph.hpp"
#include "Group.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "VcStrgy.hpp"

using namespace std;
using json = nlohmann::json;

void get_strategy(istream& stream, shared_ptr<VcStrgy>& vs) {
  json s;
  try {
    stream >> s;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (s["vaccination_strategy"].is_object()
        && s["vaccination_strategy"]["type"].is_string()) {
    json strat = s["vaccination_strategy"];
    if (strat["type"] == "nothing") {
      vs = make_shared<NothingStrategy>();
    }
    else if (strat["type"] == "random") {
      vs = make_shared<RandomStrategy>();
    }
  }

  if (s["T_v"].is_number())
    vs->time_deployed = s["T_v"];
  if (s["n_v"].is_number())
    vs->vaccines_per_day = s["n_v"];
}

void get_groups(istream& stream, vector<shared_ptr<group_t>>& groups) {
  json s;
  try {
    stream >> s;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (s["groups"].is_array()) {
    for (auto group : s["groups"]) {
      shared_ptr<group_t> graf(new group_t);
      if (group["n_i"].is_number())
        graf->n_i = group["n_i"];
      if (group["n_ai"].is_number())
        graf->n_ai = group["n_ai"];
      if (group["s"].is_number())
      graf->s = group["s"];
      if (group["p_i"].is_number())
        graf->p_i = group["p_i"];
      if (group["p_ai"].is_number())
        graf->p_ai = group["p_ai"];
      if (group["p_t"].is_number())
        graf->p_t = group["p_t"];
      if (group["p_at"].is_number())
        graf->p_at = group["p_at"];
      if (group["p_v"].is_number())
        graf->p_v = group["p_v"];
      if (group["d_v"].is_number())
        graf->d_v = group["d_v"];
      if (group["d_i"].is_number())
        graf->d_i = group["d_i"];
      if (group["d_ai"].is_number())
        graf->d_ai = group["d_ai"];
      if (group["a_p"].is_number())
        graf->a_p = group["a_p"];
      groups.push_back(graf);
    }
  }
}

void initialize_simulator_from_stream(istream& stream, Simulator& sim) {
  json s;
  try {
    stream >> s;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (s["select_all"].is_boolean())
    sim.select_all = s["select_all"];
  if (s["T"].is_number())
    sim.T = s["T"];
  if (s["initial_infections"].is_number())
    sim.initial_infections = s["initial_infections"];
  if (s["T_v"].is_number())
    sim.T_v = s["T_v"];
  if (s["n_v"].is_number())
    sim.n_v = s["n_v"];
}

void initialize_graph(json& s, Graph& graf) {
  if (s["type"] == "matrix"
      && s["size"].is_number()
      && s["distance"].is_number())
    graf.matrix_graph(s["size"], s["distance"]);
  else if (s["type"] == "file_format_simple"
           && s["file_name"].is_string())
    graf.input_from_file(s["file_name"]);
  else if (s["type"] == "nw_small_world"
           && s["l"].is_number()
           && s["k"].is_number()
           && s["p"].is_number())
    graf.nw_small_world(s["l"], s["k"], s["p"]);
  else if (s["type"] == "file_format_advanced"
           && s["file_name"].is_string()) {
    ifstream file;
    file.open(s["file_name"]);
    graf.read_generatable_graph(file);
    file.close();
  }
  else
    graf.default_graph();
}

void initialize_region_connections(json& s, Graph& graf) {
  if (s["region_connections"].is_array()) {
    vector<vector<int>> connections;
    for (auto connection_list : s["region_connections"]) {
      vector<int> c;
      for (auto entry : connection_list)
        if (entry.is_number())
          c.push_back(entry);
      connections.push_back(c);
    }
    graf.set_region_connections(connections);
  }
  else if (s["region_connections"].is_string()) {
    ifstream file;
    file.open(s["region_connections"]);
    graf.set_region_connections(file);
    file.close();
  }
  else {
    graf.default_region_connections();
  }
}

void initialize_graph_from_stream(istream& stream, Graph& graf) {
  json s;
  try {
    stream >> s;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (s["graph"].is_object() && s["graph"]["type"].is_string())
    initialize_graph(s["graph"], graf);
  else if (s["graph"].is_array()) {
    for (auto graph : s["graph"]) {
      if (graph["type"].is_string())
        initialize_graph(graph, graf);
      else
        graf.default_graph();
    }
  }
  else
    graf.default_graph();

  initialize_region_connections(s, graf);
}


json result_to_json(result_t& result) {
  json j = {
    {"s", result.s},
    {"a", result.a},
    {"i", result.i},
    {"v", result.v},
    {"r", result.r}
  };
  return j;
}

json region_result_to_json(vector<result_t>& region_result) {
  json j = {
    {"region_results", {}}
  };
  for (auto result : region_result) {
    json entry = result_to_json(result);
    j["region_results"].push_back(entry);
  }
  return j;
}

void result_to_output(ostream& stream, result_t& result) {
  json output = result_to_json(result);
  stream << output << endl;
}

void result_to_output(ostream& stream, vector<result_t>& region_result) {
  json output = region_result_to_json(region_result);
  stream << output << endl;
}

void results_to_output(ostream& stream, vector<result_t>& results) {
  json output = {
    {"results", {}}
  };
  for (auto result : results) {
    json entry = result_to_json(result);
    output["results"].push_back(entry);
  }
  stream << output << endl;
}

void results_to_output(
ostream& stream, vector<vector<result_t>>& region_results) {
  json output = { {"results", {}} };
  for (auto& result : region_results) {
    json entry = region_result_to_json(result);
    output["results"].push_back(entry);
  }
  stream << output << endl;
}

void reset_stream(istream& stream) {
  stream.clear();
  stream.seekg(0, stream.beg);
}
