#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "IOjson.hpp"
#include "Results.hpp"
using namespace std;

void Results::add_susceptible(int n) {
  results.back().s += n;
  results_by_region.back().back().s += n;
}

void Results::add_susceptible() {
  add_susceptible(1);
}

void Results::add_asymptomatic(int n) {
  results.back().a += n;
  results_by_region.back().back().a += n;
}

void Results::add_asymptomatic() {
  add_asymptomatic(1);
}

void Results::add_infected(int n) {
  results.back().i += n;
  results_by_region.back().back().i += n;
}
  
void Results::add_infected() {
  add_infected(1);
}

void Results::add_vaccinated(int n) {
  results.back().v += n;
  results_by_region.back().back().v += n;
}

void Results::add_vaccinated() {
  add_vaccinated(1);
}

void Results::add_removed(int n) {
  results.back().r += n;
  results_by_region.back().back().r += n;
}
  
void Results::add_removed() {
  add_removed(1);
}

void Results::prepare_new_region() {
  result_t new_result;
  results_by_region.back().push_back(new_result);
}

void Results::prepare_new_result() {
  result_t new_result;
  results.push_back(new_result);
  vector<result_t> new_region_results;
  results_by_region.push_back(new_region_results);
}

void Results::print(int N_population) {
    clog << "\n";
  int T = results.size();
  for (int t = 0; t < T; t++) {
    const int WDTH = 100, spcs = 8;
    for (int i = 0; i < round(WDTH*results[t].i/N_population); i++) {clog << "I";}
    int n_s_t = WDTH - round(WDTH*results[t].i/N_population) - round(WDTH*results[t].r/N_population);
    for (int i = 0; i < n_s_t; i++) {clog << " ";}
    for (int i = 0; i < round(WDTH*results[t].r/N_population); i++) {clog << "R";}
    clog << "| s a i v r(" << setw(3) << setfill(' ') << t << "): ";
    clog << setw(spcs) << setfill(' ') << results[t].s;
    clog << setw(spcs) << setfill(' ') << results[t].a;
    clog << setw(spcs) << setfill(' ') << results[t].i;
    clog << setw(spcs) << setfill(' ') << results[t].v;
    clog << setw(spcs) << setfill(' ') << results[t].r << "\n";
  }
}

void Results::save_to_file(string file_name) {
  ofstream f;
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

void Results::write_to_output_stream(ostream& stream, bool split_by_region) {
  if (split_by_region)
    results_to_output(stream, results_by_region);
  else
    results_to_output(stream, results);
}

void Results::write_last_to_output_stream(ostream& stream, bool split_by_region) {
  if (split_by_region)
    result_to_output(stream, results_by_region.back());
  else
    result_to_output(stream, results.back());
}
