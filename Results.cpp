#include <vector>
#include <iostream>
#include "Results.hpp"
#include <string>
#include <fstream>

void Results::add_susceptible(int n) {
  results.back().s += n;
}

void Results::add_susceptible() {
  add_susceptible(1);
}

void Results::add_asymptomatic(int n) {
  results.back().a += n;
}

void Results::add_asymptomatic() {
  add_asymptomatic(1);
}

void Results::add_infected(int n) {
  results.back().i += n;
}
  
void Results::add_infected() {
  add_infected(1);
}

void Results::add_vaccinated(int n) {
  results.back().v += n;
}

void Results::add_vaccinated() {
  add_vaccinated(1);
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
  for (auto result : results)
    std::cout
      << "(S, A, I, V, R): "
      << result.s << ",\t"
      << result.a << ",\t"
      << result.i << ",\t"
      << result.v << ",\t"
      << result.r << "\n";
}

void Results::save_to_file(std::string file_name) {
  std::ofstream f;
  f.open(file_name);
  for (auto result : results) {
    f <<
      result.s << " " <<
      result.a << " " <<
      result.i << " " <<
      result.v << " " <<
      result.r << "\n";
  }
  f.close();
}
