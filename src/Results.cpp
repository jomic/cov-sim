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

void Results::print(int N) {
    cout << "\n";
  int T = results.size();
  for (int t = 0; t < T; t++) {
    const int WDTH = 100, spcs = 8;
    for (int i = 0; i < round(WDTH*results[t].i/N); i++) {cout << "I";}
    int n_s_t = WDTH - round(WDTH*results[t].i/N) - round(WDTH*results[t].r/N);
    for (int i = 0; i < n_s_t; i++) {cout << " ";}
    for (int i = 0; i < round(WDTH*results[t].r/N); i++) {cout << "R";}
    cout << "| s a i v r(" << setw(3) << setfill(' ') << t << "): ";
    cout << setw(spcs) << setfill(' ') << results[t].s;
    cout << setw(spcs) << setfill(' ') << results[t].a;
    cout << setw(spcs) << setfill(' ') << results[t].i;
    cout << setw(spcs) << setfill(' ') << results[t].v;
    cout << setw(spcs) << setfill(' ') << results[t].r << "\n";
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
