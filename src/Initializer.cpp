#include <vector>
#include <iostream>
#include "Group.hpp"
#include "Simulator.hpp"
#include "../include/json.hpp"

using json = nlohmann::json;


void get_groups_from_stream(std::istream& stream, std::vector<group_t>& groups) {
  json settings;
  stream >> settings;
  // TODO
}

void initialize_simulator_from_stream(std::istream& stream, Simulator& s) {
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

void reset_stream(std::istream& stream) {
  stream.clear();
  stream.seekg(0, stream.beg);
}
