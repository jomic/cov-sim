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

void Results::plot() {
  int N_tot = results[0].s + results[0].a + results[0].i
      + results[0].v + results[0].r;
  clog << endl;
  int T = results.size();
  for (int t = 0; t < T; t++) {
    const int WDTH = 100, spcs = 8;
    for (int j = 0; j < round(WDTH*results[t].a/N_tot); j++) {clog << "A";}
    for (int j = 0; j < round(WDTH*results[t].i/N_tot); j++) {clog << "I";}
    int n_s_t = WDTH - round(WDTH*results[t].a/N_tot)
        - round(WDTH*results[t].i/N_tot) - round(WDTH*results[t].v/N_tot)
        - round(WDTH*results[t].r/N_tot); // n_s_t = # susceptible at time t
    for (int j = 0; j < n_s_t; j++) {clog << " ";} // " " = susceptible
    for (int j = 0; j < round(WDTH*results[t].v/N_tot); j++) {clog << "V";}
    for (int j = 0; j < round(WDTH*results[t].r/N_tot); j++) {clog << "R";}
    clog << "| SAIVR (" << setw(3) << setfill(' ') << t << "): ";
    clog << setw(spcs) << setfill(' ') << results[t].s;
    clog << setw(spcs) << setfill(' ') << results[t].a;
    clog << setw(spcs) << setfill(' ') << results[t].i;
    clog << setw(spcs) << setfill(' ') << results[t].v;
    clog << setw(spcs) << setfill(' ') << results[t].r << endl;
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
      result.r << endl;
  }
  f.close();
}

void Results::write_to_output(ostream& stream, bool split_by_region) {
  if (split_by_region)
    results_to_output(stream, results_by_region);
  else
    results_to_output(stream, results);
}

void Results::write_last_to_output(ostream& stream, bool split_by_region) {
  if (split_by_region)
    result_to_output(stream, results_by_region.back());
  else
    result_to_output(stream, results.back());
}
