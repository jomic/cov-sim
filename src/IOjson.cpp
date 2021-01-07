#include <fstream>
#include <iostream>
#include "../include/json.hpp"
#include <memory>
#include <vector>
#include "Graph.hpp"
#include "Group.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "VacStrat.hpp"

using namespace std;
using json = nlohmann::json;

void get_strategy(istream& stream, shared_ptr<VacStrat>& vs) {
  json json_obj;
  try {
    stream >> json_obj;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (json_obj["vaccination_strategy"].is_object()
        && json_obj["vaccination_strategy"]["type"].is_string()) {
    json json_strat = json_obj["vaccination_strategy"];
    if (json_strat["type"] == "nothing") {
      vs = make_shared<NothingStrategy>();
    }
    else if (json_strat["type"] == "random") {
      vs = make_shared<RandomStrategy>();
    }
    else if (json_strat["type"] == "low_density") {
      vs = make_shared<LowestDensityStrategy>();
    }
    else if (json_strat["type"] == "high_density") {
      vs = make_shared<HighestDensityStrategy>();
    }
  }

  if (json_obj["T_v"].is_number())
    vs->time_deployed = json_obj["T_v"];
  if (json_obj["n_v"].is_number())
    vs->vaccines_per_day = json_obj["n_v"];
}

void get_groups(istream& stream, vector<shared_ptr<Group>>& groups) {
  json json_obj;
  try {
    stream >> json_obj;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (json_obj["groups"].is_array()) {
    for (auto group : json_obj["groups"]) {
      shared_ptr<Group> graf(new Group);
      if (group["n_i"].is_number())
        graf->n_i = group["n_i"];
      if (group["n_ai"].is_number())
        graf->n_ai = group["n_ai"];
      if (group["susceptibility"].is_number())
        graf->susceptibility = group["susceptibility"];
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

void initialize_simulator(istream& stream, Simulator& sim) {
  json json_obj;
  try {
    stream >> json_obj;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (json_obj["select_all"].is_boolean())
    sim.select_all = json_obj["select_all"];
  if (json_obj["T"].is_number())
    sim.T = json_obj["T"];
  if (json_obj["initial_infections"].is_number())
    sim.initial_infections = json_obj["initial_infections"];
  if (json_obj["T_v"].is_number())
    sim.T_v = json_obj["T_v"];
  if (json_obj["n_v"].is_number())
    sim.n_v = json_obj["n_v"];
}

void initialize_graph(json& json_obj, Graph& graf) {
  if (json_obj["type"] == "matrix"
      && json_obj["size"].is_number()
      && json_obj["distance"].is_number())
    graf.matrix_graph(json_obj["size"], json_obj["distance"]);
  else if (json_obj["type"] == "file_format_simple"
           && json_obj["file_name"].is_string())
    graf.input_from_file(json_obj["file_name"]);
  else if (json_obj["type"] == "nw_small_world"
           && json_obj["N"].is_number()
           && json_obj["k"].is_number()
           && json_obj["p"].is_number())
    graf.nw_small_world(json_obj["N"], json_obj["k"], json_obj["p"]);
  else if (json_obj["type"] == "random_graph"
           && json_obj["Npop"].is_number()
           && json_obj["N0"].is_number())
    graf.random_graph(json_obj["Npop"], json_obj["N0"]);
  else if (json_obj["type"] == "file_format_advanced"
           && json_obj["file_name"].is_string()) {
    ifstream file;
    file.open(json_obj["file_name"]);
    graf.read_generatable_graph(file);
    file.close();
  }
  else
    graf.default_graph();
}

void initialize_region_connections(json& json_obj, Graph& graf) {
  if (json_obj["region_connections"].is_array()) {
    vector<vector<int>> connections;
    for (auto connection_list : json_obj["region_connections"]) {
      vector<int> c;
      for (auto entry : connection_list)
        if (entry.is_number())
          c.push_back(entry);
      connections.push_back(c);
    }
    graf.set_region_connections(connections);
  }
  else if (json_obj["region_connections"].is_string()) {
    ifstream file;
    file.open(json_obj["region_connections"]);
    graf.set_region_connections(file);
    file.close();
  }
  else {
    graf.default_region_connections();
  }
}

void initialize_graph(istream& stream, Graph& graf) {
  json json_obj;
  try {
    stream >> json_obj;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (json_obj["graph"].is_object() && json_obj["graph"]["type"].is_string())
    initialize_graph(json_obj["graph"], graf);
  else if (json_obj["graph"].is_array()) {
    for (auto graph : json_obj["graph"]) {
      if (graph["type"].is_string())
        initialize_graph(graph, graf);
      else
        graf.default_graph();
    }
  }
  else
    graf.default_graph();

  initialize_region_connections(json_obj, graf);
}

json result_to_json(Result& result) {
  json json_obj = {
    {"s", result.s_count},
    {"a", result.a_count},
    {"i", result.i_count},
    {"v", result.v_count},
    {"r", result.r_count}
  };
  return json_obj;
}

json region_result_to_json(vector<Result>& region_result) {
  json json_obj = {
    {"region_results", {}}
  };
  for (auto result : region_result) {
    json entry = result_to_json(result);
    json_obj["region_results"].push_back(entry);
  }
  return json_obj;
}

void result_to_output(ostream& stream, Result& result) {
  json output = result_to_json(result);
  stream << output << endl;
}

void result_to_output(ostream& stream, vector<Result>& region_result) {
  json output = region_result_to_json(region_result);
  stream << output << endl;
}

void results_to_output(ostream& stream, vector<Result>& results) {
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
ostream& stream, vector<vector<Result>>& region_results) {
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
