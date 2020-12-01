// Imperative and Object-Oriented Programming Methodology
// Uppsala University - 2020 Autumn
// Copyright (c) 2020 group CovSim2: Henrik Schulze,
// Christopher Rydell, Jonatan Michalak, Amina Manafli, Wenhao Zhu.
#include <iomanip>
#include <iostream>
#include <math.h>
#include <omp.h>
using namespace std;

struct node_t {
  unsigned char s:1, i:1, r:1, timeOfInfect:8;
};

struct grid_t {
  int L, N;
  node_t *nodes;
};

struct sir_t {
  int n_s, n_i, n_r;
};

void init_node(node_t *node) {
  node->s = true;
  node->i = false;
  node->r = false;
  node->timeOfInfect = 255;
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
  target->timeOfInfect = t;
}

void infect_initial(sir_t *sir, grid_t *grid) {
  sir->n_i = 4;
  sir->n_s -= 4;
  // srand(time(0));
  for (int i = 0; i < 4; i++) {infect(&grid->nodes[rand()%grid->N],0);}
}

sir_t *init_sirs(grid_t *grid, int gamma_inv, float betaD, int D0,
    int T0, int T) {
  sir_t *sirs = (sir_t *) calloc(T + 1, sizeof(sir_t));
  sirs[0].n_i = 0;
  sirs[0].n_s = grid->N;
  sirs[0].n_r = 0;
  infect_initial(&sirs[0],grid);
  return sirs;
}

void plot_grid(grid_t *grid, int L) {
  cout << "\n";
  for (int i = 0; i < L+1; i++) {cout << " -";}
  cout << "\n";
  for (int i = 0; i < L; i++) {
    cout << "|";
    for (int j=0; j<L; j++) {
      node_t *individ = &grid->nodes[grid->L*i + j];
      cout << " " << (individ->s? " ": (individ->i? "I": "R"));
    }
    cout << " |\n";
  }
  for (int i = 0; i < L+1; i++) {cout << " -";}
  cout << "\n\n";
}

void infecting(node_t *source, node_t *target, float betaD, int t) {
  if (source != target && target->s) {
    float roll = (float) rand() / (float) RAND_MAX;
    if (roll < betaD) {infect(target, t);}
  }
}

void spreading(grid_t *grid, int i, int j, int Dt, float betaD, int t) {
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
      infecting(source, target, betaD, t);
    }
  }
}

void recover(node_t *node) {
  node->i = false;
  node->r = true;
}

void recovering(node_t *node, int gamma_inv, int t) {
  if (node->timeOfInfect + gamma_inv < t) {recover(node);}
}

void iterate_for(grid_t *grid, sir_t *sir, int gamma_inv, float betaD,
    int Dt, int t, int node_i) {
  int i = node_i / grid->L;
  int j = node_i % grid->L;
  node_t *node = &grid->nodes[node_i];
  if (node->i) {
    recovering(node, gamma_inv, t);
    if (node->i && node->timeOfInfect < t) {
      spreading(grid, i, j, Dt, betaD, t);
    }
  }
  if (node -> i) sir->n_i++;
  else if (node->s) {sir->n_s++;}
}

void iterate(grid_t *grid, sir_t *sir, int gamma_inv, float betaD,
    int Dt, int t) {
  for (int node_i = 0; node_i < grid->N; node_i++) {
    iterate_for(grid, sir, gamma_inv, betaD, Dt, t, node_i);
  }
  sir->n_r = grid->N - sir->n_i - sir->n_s;
}

void plot_sir(sir_t *sir, int N, int t) {
  const int WDTH = 100, spcs = 8;
  for (int i = 0; i < round(WDTH*sir->n_i/N); i++) {cout << "I";}
  int n_s_t = WDTH - round(WDTH*sir->n_i/N) - round(WDTH*sir->n_r/N);
  for (int i = 0; i < n_s_t; i++) {cout << " ";}
  for (int i = 0; i < round(WDTH*sir->n_r/N); i++) {cout << "R";}
  cout << "| i s r(" << setw(3) << setfill(' ') << t << "): ";
  cout << setw(spcs) << setfill(' ') << sir->n_i;
  cout << setw(spcs) << setfill(' ') << sir->n_s;
  cout << setw(spcs) << setfill(' ') << sir->n_r << "\n";
}

sir_t *simulate(sir_t *sirs, grid_t *grid, int gamma_inv, float betaD,
    int D0, int T0, int T) {
  for (int t = 1; t <= T; t++) {
    const float lambda = 2.5;
    int Dt = t < T0 ? D0 : round((float) D0*exp((T0 - t)/lambda));
    iterate(grid,&sirs[t],gamma_inv, betaD, Dt, t);
    plot_sir(&sirs[t], grid->N, t);
    if (grid->L<71 && t==30) {plot_grid(grid, grid->L);} // 30 = arbitrary
  }
  return sirs;
}

void plot_sirs(sir_t *sirs, grid_t *grid, int T) {
  for (int t = 0; t <= T; t++) {plot_sir(&sirs[t], grid->N, t);}
}

void omp_cores() {
  cout << "\n" << "omp_get_max_threads: " << omp_get_max_threads() << "\n";
}

int main() {
  int L = 7, gamma_inv = 14, D0 = 2, T0 = 50, T = 50;
  float betaC = 0.25, betaD = betaC/((2*D0+1)*(2*D0+1) - 1);
  // betaD = 0.25;
  omp_set_num_threads(2);
  omp_cores();

  grid_t *grid = create_grid(L);
  sir_t *sirs = init_sirs(grid, gamma_inv, betaD, D0, T0, T);
  if (L<71) {plot_grid(grid, L);}
  plot_sir(&sirs[0], grid->N, 0);
  sirs = simulate(sirs, grid, gamma_inv, betaD, D0, T0, T);
  // plot_sirs(sirs, grid, T);
  if (L<71) plot_grid(grid, L);

  free(sirs);
  destroy_grid(grid);
  return EXIT_SUCCESS;
}
// make clean && make matrixC19,L-7,D0-2
