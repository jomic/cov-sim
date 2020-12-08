#pragma once

class Simulator;
#include <vector>
#include <iostream>
#include "Group.hpp"

/*
  Fill a vector with groups based on information from json stream
 */
void get_groups_from_stream(std::istream& stream, std::vector<group_t>& groups);

/*
  Set simulator parameters based on information from json stream
 */
void initialize_simulator_from_stream(std::istream& stream, Simulator& s);

/*
  Write a single result to an output stream as json
 */
void write_result_to_output_stream(std::ostream& stream, result_t& result);

/*
  Write an vector of results to an output stream as json
 */
void write_results_to_output_stream(std::ostream& stream, std::vector<result_t>& results);

/*
  Reset an input stream so it can be used again
 */
void reset_stream(std::istream& stream);
