#include <vector>
#include <iostream>
#include "Results.hpp"

void Results::add_susceptible(int n) {
  results.back().s += n;
}

void Results::add_susceptible() {
  add_susceptible(1);
}

void Results::add_infected(int n) {
  results.back().i += n;
}
  
void Results::add_infected() {
  add_infected(1);
}

void Results::add_removed(int n) {
  results.back().r += n;
}
  
void Results::add_removed() {
  add_removed(1);
}

void Results::prepare_new_result() {
  result_t new_result;
  results.push_back(new_result);
}

void Results::print() {
  for (auto it = results.begin(); it != results.end(); ++it)
    std::cout
      << "(S, I, R): "
      << it->s << ",\t"
      << it->i << ",\t"
      << it->r << "\n";
}
