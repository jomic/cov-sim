// Imperative and Object-Oriented Programming Methodology
// Uppsala University - 2020 Autumn
// Copyright (c) 2020 group CovSim2: Henrik Schulze,
// Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
#include <iomanip>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <set>
#include <string>
using namespace std;

struct Agent {
  unsigned char is_susceptbl:1, is_infectd:1, is_recovrd:1, infected_on:8;
};

struct Grid {
  int L, N;
  Agent *agents;
};

struct Result { int count_s, count_i, count_r; };

void init_agent(Agent *agent) {
  agent->is_susceptbl = true;
  agent->is_infectd = false;
  agent->is_recovrd = false;
  agent->infected_on = 255;
}

// Create a grid for the infection to spread in:
Grid *create_grid(int L) {
  Grid *grid = (Grid *) calloc(1, sizeof(Grid));
  grid->L = L;
  grid->N = L*L;
  Agent *agents = (Agent *) calloc(grid->N, sizeof(Agent));
  grid->agents = agents;
  for (int j = 0; j < grid->N; j++) {init_agent(&agents[j]);}
  return grid;
}

void destroy_grid(Grid *grid) {
  free(grid->agents);
  free(grid);
}

void infect(Agent *target, int t) {
  target->is_susceptbl = false;
  target->is_infectd = true;
  target->infected_on = t;
}

set<int> unique_rand_numbers(int numbr, int max) {
  set<int> numbers;
  if (numbr >= max) {
    cout << "random_numbers: numbr cant be larger than total" << endl;
    return numbers;
  }
  while ((int)numbers.size() < numbr) {
      numbers.insert(rand() % max);
    }
  return numbers;
}

void infect_initial(int init_infected, Result *result, Grid *grid,
    bool random_seed) {
  result->count_i = init_infected;
  result->count_s -= init_infected;
  if (random_seed) {srand(time(NULL));}
  set<int> random_nmbrs = unique_rand_numbers(init_infected, grid->N);
  for (auto j : random_nmbrs) {
    infect(&grid->agents[j],0);
  }
}

Result *init_results(int init_infected, Grid *grid, int T,
    bool random_seed) {
  Result *results = (Result *) calloc(T + 1, sizeof(Result));
  results[0].count_i = 0;
  results[0].count_s = grid->N;
  results[0].count_r = 0;
  infect_initial(init_infected, &results[0], grid, random_seed);
  return results;
}

void plot_grid(Grid *grid, int L) {
  cout << endl;
  for (int j = 0; j < L+1; j++) {cout << " -";}
  cout << endl;
  for (int j = 0; j < L; j++) {
    cout << "|";
    for (int k=0; k<L; k++) {
      Agent *agent = &grid->agents[grid->L*j + k];
      cout << " " << (agent->is_susceptbl?" ":(agent->is_infectd?"I":"R"));
    }
    cout << " |" << endl;
  }
  for (int j = 0; j < L+1; j++) {cout << " -";}
  cout << endl << endl;
}

void try_infecting(Agent *source, Agent *target, float betaD, int t) {
  if (source!=target && target->is_susceptbl) {
    float roll = (float) rand() / (float) RAND_MAX;
    if (roll < betaD) {infect(target, t);}
  }
}

void try_spreading(Grid *grid, int j, int k, int Dt, float betaD, int t) {
  // Determine the bounds of Dt:
  int d_right = min(k + Dt + 1, grid->L);
  int d_top = max(j - Dt, 0);
  int d_left = max(k - Dt, 0);
  int d_bottom = min(j + Dt + 1, grid->L);

  // Call function on each agent within Dt:
  Agent *source = &grid->agents[grid->L*j + k];
  for (int l = d_top; l < d_bottom; l++) {
    for (int m = d_left; m < d_right; m++) {
      Agent *target = &grid->agents[grid->L*l + m];
      try_infecting(source, target, betaD, t);
    }
  }
}

void recover(Agent *agent) {
  agent->is_infectd = false;
  agent->is_recovrd = true;
}

void recovering(Agent *agent, int days_sick, int t) {
  if (agent->infected_on + days_sick < t) {recover(agent);}
}

void iterate_for(Grid *grid, Result *result, int days_sick, float betaD,
    int Dt, int t, int agent_i) {
  int j = agent_i / grid->L;
  int k = agent_i % grid->L;
  Agent *agent = &grid->agents[agent_i];
  if (agent->is_infectd) {
    recovering(agent, days_sick, t);
    if (agent->is_infectd && agent->infected_on < t) {
      try_spreading(grid, j, k, Dt, betaD, t);
    }
  }
  if (agent->is_infectd) result->count_i++;
  else if (agent->is_susceptbl) {result->count_s++;}
}

void iterate(Grid *grid, Result *result, int days_sick, float betaD,
    int Dt, int t) {
  for (int agent_i = 0; agent_i < grid->N; agent_i++) {
    iterate_for(grid, result, days_sick, betaD, Dt, t, agent_i);
  }
  result->count_r = grid->N - result->count_i - result->count_s;
}

void plot_result(Result *result, int N, int t) {
  const int WDTH = 100, spcs = 8;
  for (int j = 0; j < round(WDTH*result->count_i/N); j++) {cout << "I";}
  int n_s_t = WDTH // n_s_t = number of susceptible at time t
      - round(WDTH*result->count_i/N) - round(WDTH*result->count_r/N);
  for (int j = 0; j < n_s_t; j++) {cout << " ";}
  for (int j = 0; j < round(WDTH*result->count_r/N); j++) {cout << "R";}
  cout << "| ISR (" << setw(3) << setfill(' ') << t << "): ";
  cout << setw(spcs) << setfill(' ') << result->count_i;
  cout << setw(spcs) << setfill(' ') << result->count_s;
  cout << setw(spcs) << setfill(' ') << result->count_r << endl;
}

Result *simulate(Result *results, Grid *grid, int days_sick,
    float betaD, int D0, int T0, int T) {
  for (int t = 1; t <= T; t++) {
    const float lambda = 2.5;
    int Dt = t < T0 ? D0 : round((float) D0*exp((T0 - t)/lambda));
    iterate(grid,&results[t],days_sick, betaD, Dt, t);
    plot_result(&results[t], grid->N, t);
    // if (grid->L<71 && t==20) {plot_grid(grid, grid->L);} // 20 = arbitrary
  }
  return results;
}

void plot_results(Result *results, int N, int T) {
  for (int t = 0; t <= T; t++) {plot_result(&results[t], N, t);}
}

void omp_cores() {
  cout << endl << "omp_get_max_threads: " << omp_get_max_threads() << endl;
}

int main(int argc, char *argv[]) {
  omp_cores();
  int init_infected = 4, L = 7, days_sick = 14, D0 = 2, T = 50, T0 = 0;
  float betaC = 0.25;
  bool random_seed = false;

  if (argc > 8) {
    cout << "Usage: " << argv[0] << " L D0 t" << endl;
    return 1;
  } else if (argc == 8) {
    init_infected = stoi(argv[1]);
    cout << "init_infected = " << init_infected;
    L             = stoi(argv[2]);
    days_sick     = stoi(argv[3]);
    D0            = stoi(argv[4]);
    T             = stoi(argv[5]);
    // T0            = stoi(argv[6]);
    betaC         = stof(argv[6]);
    random_seed   = (string(argv[7]).compare("true") == 0)?true:false;
  }
  T0 = T;
  float betaD = betaC/((2*D0+1)*(2*D0+1) - 1);

  Grid *grid = create_grid(L);
  Result *results = init_results(init_infected, grid, T, random_seed);
  if (L<71) {plot_grid(grid, L);} // The grid BEFORE the simulation.
  plot_result(&results[0], grid->N, 0);
  results = simulate(results, grid, days_sick, betaD, D0, T0, T);
  if (L<71) plot_grid(grid, L); // The grid AFTER the simulation.
  // plot_results(results, grid->N, T); // Plot results for ALL time steps.

  free(results);
  destroy_grid(grid);
  return EXIT_SUCCESS;
}
// make clean_binaries && make matrixC19
// bin/matrixC19 2 5 14 1 50 0.4 true
// bin/matrixC19 4 7 14 2 60 0.25 true
// bin/matrixC19 4 51 14 3 150 0.25 true
// bin/matrixC19 4 51 14 2 180 0.25 true
