#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "Results.hpp"
using namespace std;

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

void Results::print(int N) {
    cout << "\n";
  int T = results.size();
  // printf("T = %d\n", T);
  // printf("N = %d\n", N);
  for (int t = 0; t < T; t++) {
    const int WDTH = 100, spcs = 8;
    for (int i = 0; i < round(WDTH*results[t].i/N); i++) {cout << "I";}
    int n_s_t = WDTH - round(WDTH*results[t].i/N) - round(WDTH*results[t].r/N);
    for (int i = 0; i < n_s_t; i++) {cout << " ";}
    for (int i = 0; i < round(WDTH*results[t].r/N); i++) {cout << "R";}
    cout << "| i s r(" << setw(3) << setfill(' ') << t << "): ";
    cout << setw(spcs) << setfill(' ') << results[t].i;
    cout << setw(spcs) << setfill(' ') << results[t].s;
    cout << setw(spcs) << setfill(' ') << results[t].r << "\n";
  }
}

void Results::save_to_file(std::string file_name) {
  std::ofstream f;
  f.open(file_name);
  for (auto result : results) {
    f <<
      result.s << " " <<
      result.i << " " <<
      result.r << "\n";
  }
  f.close();
}
