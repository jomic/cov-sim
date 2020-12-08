#include <vector>
#include <iostream>
#include "Group.hpp"
#include "Simulator.hpp"
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;


void get_groups_from_stream(istream& stream, vector<group_t>& groups) {
  json settings;
  stream >> settings;

  if (settings["groups"].is_array()) {
    for (auto group : settings["groups"]) {
      group_t g;
      if (settings["n_i"].is_number())
	g.n_i = settings["n_i"];
      if (settings["n_ai"].is_number())
	g.n_ai = settings["n_ai"];
      if (settings["s"].is_number())
	g.s = settings["s"];
      if (settings["p_i"].is_number())
	g.p_i = settings["p_i"];
      if (settings["p_ai"].is_number())
	g.p_ai = settings["p_ai"];
      if (settings["p_v"].is_number())
	g.p_v = settings["p_v"];
      if (settings["d_v"].is_number())
	g.d_v = settings["d_v"];
      if (settings["d_i"].is_number())
	g.d_i = settings["d_i"];
      if (settings["d_ai"].is_number())
	g.d_ai = settings["d_ai"];
      if (settings["a_p"].is_number())
	g.a_p = settings["a_p"];
      groups.push_back(g);
    }
  }
}

void initialize_simulator_from_stream(istream& stream, Simulator& s) {
  json settings;
  stream >> settings;

  if (settings["T"].is_number())
    s.T = settings["T"];
  if (settings["N"].is_number())
    s.N = settings["N"];
  if (settings["T_v"].is_number())
    s.T_v = settings["T_v"];
  if (settings["n_v"].is_number())
    s.n_v = settings["n_v"];
}

void reset_stream(istream& stream) {
  stream.clear();
  stream.seekg(0, stream.beg);
}
