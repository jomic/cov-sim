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
  results.back().s_count += n;
  results_by_region.back().back().s_count += n;
}

void Results::add_susceptible() {
  add_susceptible(1);
}

void Results::add_asymptomatic(int n) {
  results.back().a_count += n;
  results_by_region.back().back().a_count += n;
}

void Results::add_asymptomatic() {
  add_asymptomatic(1);
}

void Results::add_infected(int n) {
  results.back().i_count += n;
  results_by_region.back().back().i_count += n;
}

void Results::add_infected() {
  add_infected(1);
}

void Results::add_vaccinated(int n) {
  results.back().v_count += n;
  results_by_region.back().back().v_count += n;
}

void Results::add_vaccinated() {
  add_vaccinated(1);
}

void Results::add_removed(int n) {
  results.back().r_count += n;
  results_by_region.back().back().r_count += n;
}

void Results::add_removed() {
  add_removed(1);
}

void Results::prepare_new_region() {
  Result new_result;
  results_by_region.back().push_back(new_result);
}

void Results::prepare_new_result() {
  Result new_result;
  results.push_back(new_result);
  vector<Result> new_region_results;
  results_by_region.push_back(new_region_results);
}

void Results::plot() {
  int N_tot = results[0].s_count + results[0].a_count + results[0].i_count
      + results[0].v_count + results[0].r_count;
  clog << endl;
  int T = results.size();
  for (int t = 0; t < T; t++) {
    const int WDTH = 100, spcs = 8;
    for (int j = 0; j < round(WDTH*results[t].a_count/N_tot); j++)
      { clog << "A"; }
    for (int j = 0; j < round(WDTH*results[t].i_count/N_tot); j++)
      { clog << "I"; }
    int n_s_t = WDTH // n_s_t = number of susceptible agents at time t
        - round(WDTH*results[t].a_count/N_tot)
        - round(WDTH*results[t].i_count/N_tot)
        - round(WDTH*results[t].v_count/N_tot)
        - round(WDTH*results[t].r_count/N_tot);
    for (int j = 0; j < n_s_t; j++) { clog << " "; } // " " = susceptible
    for (int j = 0; j < round(WDTH*results[t].v_count/N_tot); j++)
      { clog << "V"; }
    for (int j = 0; j < round(WDTH*results[t].r_count/N_tot); j++)
      { clog << "R"; }
    clog << "| SAIVR (" << setw(3) << setfill(' ') << t << "): ";
    clog << setw(spcs) << setfill(' ') << results[t].s_count;
    clog << setw(spcs) << setfill(' ') << results[t].a_count;
    clog << setw(spcs) << setfill(' ') << results[t].i_count;
    clog << setw(spcs) << setfill(' ') << results[t].v_count;
    clog << setw(spcs) << setfill(' ') << results[t].r_count << endl;
  }
  // // clog << endl;
}

void Results::save_to_file(string file_name) {
  ofstream ofs;
  ofs.open(file_name);
  for (auto result : results) {
    ofs <<
      result.s_count << " " <<
      result.a_count << " " <<
      result.i_count << " " <<
      result.v_count << " " <<
      result.r_count << endl;
  }
  ofs.close();
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
