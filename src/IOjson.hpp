#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Graph.hpp"
#include "Group.hpp"
class Simulator;
#include "VacStrat.hpp"

/**
  Initialize a vaccination strategy from json stream
 */
void get_strategy(std::istream& stream, std::shared_ptr<VacStrat>& vs);

/**
  Fill a vector with groups based on information from json stream
 */
void get_groups(std::istream& stream, std::vector<std::shared_ptr<group_t>>& groups);

/**
  Set simulator parameters based on information from json stream
 */
void initialize_simulator(std::istream& stream, Simulator& s);

/**
  Make the appropriate calls to a graph for generating a network
 */
void initialize_graph(std::istream& stream, Graph& graf);

/**
  Write a single result to an output stream as json
 */
void result_to_output(std::ostream& stream, std::vector<result_t>& result);
void result_to_output(std::ostream& stream, result_t& result);

/**
  Write an vector of results to an output stream as json
 */
void results_to_output(std::ostream& stream, std::vector<std::vector<result_t>>& results);
void results_to_output(std::ostream& stream, std::vector<result_t>& results);

/**
  Reset an input stream so it can be used again
 */
void reset_stream(std::istream& stream);
