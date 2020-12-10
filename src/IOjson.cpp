#include <iostream>
#include <vector>
#include "Graph.hpp"
#include "Group.hpp"
#include "Results.hpp"
#include "Simulator.hpp"
#include "../include/json.hpp"
using namespace std;
using json = nlohmann::json;


void get_groups_from_stream(istream& stream, vector<group_t>& groups) {
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
      group_t g;
      if (s["n_i"].is_number())
	g.n_i = s["n_i"];
      if (s["n_ai"].is_number())
	g.n_ai = s["n_ai"];
      if (s["s"].is_number())
	g.s = s["s"];
      if (s["p_i"].is_number())
	g.p_i = s["p_i"];
      if (s["p_ai"].is_number())
	g.p_ai = s["p_ai"];
      if (s["p_v"].is_number())
	g.p_v = s["p_v"];
      if (s["d_v"].is_number())
	g.d_v = s["d_v"];
      if (s["d_i"].is_number())
	g.d_i = s["d_i"];
      if (s["d_ai"].is_number())
	g.d_ai = s["d_ai"];
      if (s["a_p"].is_number())
	g.a_p = s["a_p"];
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
  else
    g.default_graph();
}

void initialize_graph_from_stream(istream& stream, Graph& g, vector<group_t>& groups) {
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
