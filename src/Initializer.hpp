#pragma once

class Simulator;
#include <vector>
#include <iostream>
#include "Group.hpp"

/*
  Fill a vector with groups based on information from json file
 */
void get_groups_from_stream(std::istream& stream, std::vector<group_t>& groups);

/*
  Set simulator parameters based on information from json file
 */
void initialize_simulator_from_stream(std::istream& stream, Simulator& s);

/*
  Reset an input stream so it can be used again
 */
void reset_stream(std::istream& stream);
