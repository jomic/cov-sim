// Imperative and Object-Oriented Programming Methodology
// Uppsala University - 2020 Autumn
// Copyright (c) 2020 group CovSim2: Henrik Schulze,
// Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
#include <iomanip>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <string>
using namespace std;

struct node_t {
  unsigned char s:1, i:1, r:1, infected_on:8;
};

struct grid_t {
  int L, N;
  node_t *nodes;
};

struct result_t {
int s, i, r;
};

void init_node(node_t *node) {
  node->s = true;
  node->i = false;
  node->r = false;
  node->infected_on = 255;
}

// Create a grid for the infection to spread in:
grid_t *create_grid(int L) {
  grid_t *grid = (grid_t *) calloc(1, sizeof(grid_t));
  grid->L = L;
  grid->N = L*L;
  node_t *nodes = (node_t *) calloc(grid->N, sizeof(node_t));
  grid->nodes = nodes;
  for (int i = 0; i < grid->N; i++) {init_node(&nodes[i]);}
  return grid;
}

void destroy_grid(grid_t *grid) {
  free(grid->nodes);
  free(grid);
}

void infect(node_t *target, int t) {
  target->s = false;
  target->i = true;
  target->infected_on = t;
}

void infect_initial(int init_infected, result_t *result, grid_t *grid,
    bool random_seed) {
  result->i = init_infected;
  result->s -= init_infected;
  if (random_seed) {srand(time(NULL));}
  for (int i = 0; i < init_infected; i++) {
    infect(&grid->nodes[rand()%grid->N],0);
  }
}

result_t *init_results(int init_infected, grid_t *grid, int T,
    bool random_seed) {
  result_t *results = (result_t *) calloc(T + 1, sizeof(result_t));
  results[0].i = 0;
  results[0].s = grid->N;
  results[0].r = 0;
  infect_initial(init_infected, &results[0], grid, random_seed);
  return results;
}

void plot_grid(grid_t *grid, int L) {
  cout << endl;
  for (int i = 0; i < L+1; i++) {cout << " -";}
  cout << endl;
  for (int i = 0; i < L; i++) {
    cout << "|";
    for (int j=0; j<L; j++) {
      node_t *individ = &grid->nodes[grid->L*i + j];
      cout << " " << (individ->s? " ": (individ->i? "I": "R"));
    }
    cout << " |" << endl;
  }
  for (int i = 0; i < L+1; i++) {cout << " -";}
  cout << endl << endl;
}

void try_infecting(node_t *source, node_t *target, float betaD, int t) {
  if (source != target && target->s) {
    float roll = (float) rand() / (float) RAND_MAX;
    if (roll < betaD) {infect(target, t);}
  }
}

void try_spreading(grid_t *grid, int i, int j, int Dt, float betaD, int t) {
  // Determine the bounds of Dt:
  int d_right = min(j + Dt + 1, grid->L);
  int d_top = max(i - Dt, 0);
  int d_left = max(j - Dt, 0);
  int d_bottom = min(i + Dt + 1, grid->L);

  // Call function on each node within Dt:
  node_t *source = &grid->nodes[grid->L*i + j];
  for (int k = d_top; k < d_bottom; k++) {
    for (int l = d_left; l < d_right; l++) {
      node_t *target = &grid->nodes[grid->L*k + l];
      try_infecting(source, target, betaD, t);
    }
  }
}

void recover(node_t *node) {
  node->i = false;
  node->r = true;
}

void recovering(node_t *node, int days_sick, int t) {
  if (node->infected_on + days_sick < t) {recover(node);}
}

void iterate_for(grid_t *grid, result_t *result, int days_sick, float betaD,
    int Dt, int t, int node_i) {
  int i = node_i / grid->L;
  int j = node_i % grid->L;
  node_t *node = &grid->nodes[node_i];
  if (node->i) {
    recovering(node, days_sick, t);
    if (node->i && node->infected_on < t) {
      try_spreading(grid, i, j, Dt, betaD, t);
    }
  }
  if (node -> i) result->i++;
  else if (node->s) {result->s++;}
}

void iterate(grid_t *grid, result_t *result, int days_sick, float betaD,
    int Dt, int t) {
  for (int node_i = 0; node_i < grid->N; node_i++) {
    iterate_for(grid, result, days_sick, betaD, Dt, t, node_i);
  }
  result->r = grid->N - result->i - result->s;
}

void plot_result(result_t *result, int N, int t) {
  const int WDTH = 100, spcs = 8;
  for (int i = 0; i < round(WDTH*result->i/N); i++) {cout << "I";}
  int n_s_t = WDTH - round(WDTH*result->i/N) - round(WDTH*result->r/N);
  for (int i = 0; i < n_s_t; i++) {cout << " ";}
  for (int i = 0; i < round(WDTH*result->r/N); i++) {cout << "R";}
  cout << "| i s r(" << setw(3) << setfill(' ') << t << "): ";
  cout << setw(spcs) << setfill(' ') << result->i;
  cout << setw(spcs) << setfill(' ') << result->s;
  cout << setw(spcs) << setfill(' ') << result->r << endl;
}

result_t *simulate(result_t *results, grid_t *grid, int days_sick,
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

void plot_results(result_t *results, int N, int T) {
  for (int t = 0; t <= T; t++) {plot_result(&results[t], N, t);}
}

void omp_cores() {
  cout << endl << "omp_get_max_threads: " << omp_get_max_threads() << endl;
}

int main(int argc, char *argv[]) {
  omp_cores();
  int init_infected = 4, L = 7, days_sick = 14, D0 = 2, T0 = 60, T = 60;
  float betaC = 0.25;
  bool random_seed = false;

    printf("argc = %d\n", argc);

  if (argc > 9) {
    cout << "Usage: " << argv[0] << " L D0 t" << endl;
    return 1;
  } else if (argc == 9) {
    init_infected = stoi(argv[1]);
    printf("init_infected = %d", init_infected);
    L             = stoi(argv[2]);
    days_sick     = stoi(argv[3]);
    D0            = stoi(argv[4]);
    T0            = stoi(argv[5]);
    T             = stoi(argv[6]);
    betaC         = stof(argv[7]);
    random_seed   = (string(argv[8]).compare("true") == 0)?true:false;
  }
  float betaD = betaC/((2*D0+1)*(2*D0+1));

  grid_t *grid = create_grid(L);
  result_t *results = init_results(init_infected, grid, T, random_seed);
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
// bin/matrixC19 2 5 14 1 50 50 0.4 true
