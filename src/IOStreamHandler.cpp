#include <vector>
#include <iostream>
#include <fstream>
#include "Group.hpp"
#include "Graph.hpp"
#include "Simulator.hpp"
#include "Results.hpp"
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;


void get_groups_from_stream(istream& stream, vector<shared_ptr<group_t>>& groups) {
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
      shared_ptr<group_t> g(new group_t);
      if (group["n_i"].is_number())
	g->n_i = group["n_i"];
      if (group["n_ai"].is_number())
	g->n_ai = group["n_ai"];
      if (group["s"].is_number())
	g->s = group["s"];
      if (group["p_i"].is_number())
	g->p_i = group["p_i"];
      if (group["p_ai"].is_number())
	g->p_ai = group["p_ai"];
      if (group["p_v"].is_number())
	g->p_v = group["p_v"];
      if (group["d_v"].is_number())
	g->d_v = group["d_v"];
      if (group["d_i"].is_number())
	g->d_i = group["d_i"];
      if (group["d_ai"].is_number())
	g->d_ai = group["d_ai"];
      if (group["a_p"].is_number())
	g->a_p = group["a_p"];
      groups.push_back(g);
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
  if (s["N"].is_number())
    sim.N = s["N"];
  if (s["T_v"].is_number())
    sim.T_v = s["T_v"];
  if (s["n_v"].is_number())
    sim.n_v = s["n_v"];
}

void initialize_graph(json& s, Graph& g) {
  if (s["type"] == "matrix"
      && s["size"].is_number()
      && s["distance"].is_number())
    g.matrix_graph(s["size"], s["distance"]);
  else if (s["type"] == "file"
	   && s["file_name"].is_string())
    g.input_from_file(s["file_name"]);
  else if (s["type"] == "file_2"
	   && s["file_name"].is_string()) {
    ifstream file;
    file.open(s["file_name"]);
    g.read_generatable_graph(file);
    file.close();
  }
  else
    g.default_graph();
}

void initialize_graph_from_stream(istream& stream, Graph& g) {
  json s;
  try {
    stream >> s;
  }
  catch (const exception& e){
    cerr << "Something went wrong when parsing the JSON settings." << endl;
    return;
  }

  if (s["graph"].is_object() && s["graph"]["type"].is_string())
    initialize_graph(s["graph"], g);
  else if (s["graph"].is_array()) {
    for (auto graph : s["graph"]) {
      if (graph["type"].is_string())
	initialize_graph(graph, g);
      else
	g.default_graph();
    }
  }
  else
    g.default_graph();
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

void write_result_to_output_stream(ostream& stream, result_t& result) {
  json output = result_to_json(result);
  stream << output << endl;
}

void write_results_to_output_stream(ostream& stream, vector<result_t>& results) {
  json output = {
    {"results", {}}
  };
  for (auto result : results) {
    json entry = result_to_json(result);
    output["results"].push_back(entry);
  }
  stream << output << endl;
}

void reset_stream(istream& stream) {
  stream.clear();
  stream.seekg(0, stream.beg);
}
