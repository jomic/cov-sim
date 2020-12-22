#pragma once

class Simulator;
#include <vector>
#include <memory>
#include <iostream>
#include "Group.hpp"
#include "Graph.hpp"

/*
  Fill a vector with groups based on information from json stream
 */
void get_groups_from_stream(std::istream& stream, std::vector<std::shared_ptr<group_t>>& groups);

/*
  Set simulator parameters based on information from json stream
 */
void initialize_simulator_from_stream(std::istream& stream, Simulator& s);

/*
  Make the appropriate calls to a graph for generating a network
 */
void initialize_graph_from_stream(std::istream& stream, Graph& g);

/*
  Write a single result to an output stream as json
 */
void write_result_to_output_stream(std::ostream& stream, std::vector<result_t>& result);
void write_result_to_output_stream(std::ostream& stream, result_t& result);

/*
  Write an vector of results to an output stream as json
 */
void write_results_to_output_stream(std::ostream& stream, std::vector<std::vector<result_t>>& results);
void write_results_to_output_stream(std::ostream& stream, std::vector<result_t>& results);

/*
  Reset an input stream so it can be used again
 */
void reset_stream(std::istream& stream);
