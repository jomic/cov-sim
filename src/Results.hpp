#pragma once
#include <vector>
#include <string>

struct result_t {
  int s{0};
  int i{0};
  int r{0};
  };

class Results {
private:
  std::vector<result_t> results;
public:
  void add_susceptible(int n);
  void add_susceptible();
  void add_infected(int n);
  void add_infected();
  void add_removed(int n);
  void add_removed();
  void prepare_new_result();
  void print(int N);
  void save_to_file(std::string file_name);
};
