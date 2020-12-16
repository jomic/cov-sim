#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "Results.hpp"
#include "IOStreamHandler.hpp"
using namespace std;

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

void Results::print(int Npopulation) { // TODO, no arg in this function!
  clog << endl;
  int T = results.size(), N = Npopulation;
  for (int t = 0; t < T; t++) {
    const int WDTH = 100, spcs = 8;
    for (int i = 0; i < round(WDTH*results[t].i/N); i++) {clog << "I";}
    int n_s_t = WDTH - round(WDTH*results[t].i/N) - round(WDTH*results[t].r/N);
    for (int i = 0; i < n_s_t; i++) {clog << " ";}
    for (int i = 0; i < round(WDTH*results[t].r/N); i++) {clog << "R";}
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

void Results::write_to_output_stream(ostream& stream) {
  write_results_to_output_stream(stream, results);
}
