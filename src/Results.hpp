#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::vector;

struct Result {
  int s_count{0};
  int a_count{0};
  int i_count{0};
  int v_count{0};
  int r_count{0};
};

class Results {
private:
  vector<Result> results;
  vector<vector<Result>> results_by_region;
public:
  void add_susceptible(int n);
  void add_susceptible();
  void add_asymptomatic(int n);
  void add_asymptomatic();
  void add_infected(int n);
  void add_infected();
  void add_vaccinated(int n);
  void add_vaccinated();
  void add_removed(int n);
  void add_removed();
  void prepare_new_region();
  void prepare_new_result();
  void plot();
  void save_to_file(std::string file_name);
  void write_to_output(std::ostream& stream, bool split_by_region);
  void write_last_to_output(std::ostream& stream, bool split_by_region);
};
