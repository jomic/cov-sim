#include <iostream>
#include <stdlib.h>
#include <math.h>

struct node_t {
  char exists : 1;
  char s : 1;
  char i : 1;
  char r : 1;
  unsigned char infected_on : 4;
};

struct grid_t {
  int l;
  int n;
  node_t *nodes;
};

struct result_t {
  int n_s;
  int n_i;
  int n_r;
};

void init_node(node_t *node, bool infected) {
  node->exists = true;
  node->s = !infected;
  node->i = infected;
  node->r = false;
  node->infected_on = 0;
}

void draw_node(node_t node) {
  if (!node.exists)
    std::cout << " ";
  else if (node.s)
    std::cout << "o";
  else if (node.i)
    std::cout << "x";
  else
    std::cout << ".";
}

// Create a grid for the infection to spread in
grid_t *create_grid(int l) {
  grid_t *grid = (grid_t *) calloc(1, sizeof(grid_t));
  grid->l = l;
  grid->n = l * l;
  node_t *nodes = (node_t *) calloc(grid->n, sizeof(node_t));
  grid->nodes = nodes;
  for (int i = 0; i < grid->n; i++)
    init_node(&nodes[i], false);
  return grid;
}

void draw_grid(grid_t *grid) {
  int l = grid->l;
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < l; j++)
      draw_node(grid->nodes[l * i + j]);
    std::cout << "\n";
  }
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

void recover(node_t *node) {
  node->i = false;
  node->r = true;
}

void try_infecting(node_t *source, node_t *target, float beta, int t) {
  if (source != target && target->s) {
    float roll = (float) rand() / (float) RAND_MAX;
    if (roll < beta)
      infect(target, t);
  }
}

void try_recovering(node_t *node, int gamma_inv, int t) {
  if (node->infected_on + gamma_inv < t)
    recover(node);
}

void try_spreading(grid_t *grid, int i, int j, int d, float beta, int t) {
  // Detrmine the bounds of d
  int d_right = std::min(j + d + 1, grid->l);
  int d_top = std::max(i - d, 0);
  int d_left = std::max(j - d, 0);
  int d_bottom = std::min(i + d + 1, grid->l);

  // Call function on each node within d
  node_t *source = &grid->nodes[grid->l * i + j];
  for (int m = d_top; m < d_bottom; m++)
    for (int n = d_left; n < d_right; n++) {
      node_t *target = &grid->nodes[grid->l * m + n];
      try_infecting(source, target, beta, t);
    }
}

void iterate(grid_t *grid, result_t *result, int gamma_inv, float beta, int d, int t) {
  for (int node_idx = 0; node_idx < grid->n; node_idx++) {
    int i = node_idx / grid->l;
    int j = node_idx % grid->l;
    node_t *node = &grid->nodes[node_idx];
    if (node->i) {
      try_recovering(node, gamma_inv, t);
      if (node->i && node->infected_on < t)
        try_spreading(grid, i, j, d, beta, t);
    }
    if (node -> i)
      result->n_i++;
    else if (node->s)
      result->n_s++;
    else if (node->r)
      result->n_r++;
  }
}

void infect_initial(result_t *result, grid_t *grid) {
  result->n_s -= 4;
  result->n_r = 4;
  for (int i = 0; i < 4; i++)
    infect(&grid->nodes[rand() % grid->n], 0);
}

result_t *simulate(grid_t *grid, int gamma_inv, float beta, int d_0, int t_0, int iters) {
  result_t *results = (result_t *) calloc(iters + 1, sizeof(result_t));
  results[0].n_s = grid->n;
  results[0].n_i = 0;
  results[0].n_r = 0;

  infect_initial(&results[0], grid);
  
  for (int t = 1; t <= iters; t++) {
    int d = t < t_0 ? d_0 : std::round((float) d_0 * std::exp((t_0 - t) / 2.5));
    //draw_grid(grid);
    //std::cout << "\n\n";
    iterate(grid, &results[t], gamma_inv, beta, d, t);
  }
  return results;
}

void plot_results(result_t *results, int iters) {
  int n = results[0].n_s + results[0].n_i + results[0].n_r;
  for (int t = 0; t <= iters; t++) {
    for (int i = 0; i < (80 * results[t].n_i) / n; i++)
      std::cout << "X";
    for (int i = 0; i < (80 * results[t].n_s) / n; i++)
      std::cout << " ";
    for (int i = 0; i < (80 * results[t].n_r) / n; i++)
      std::cout << "-";
    std::cout << "(s, i, r): " << results[t].n_s << " " << results[t].n_i << " " << results[t].n_r << "\n";
  }
}

int main() {
  int l = 300;
  int gamma_inv = 14;
  float beta = 0.0015f;
  int d_0 = 15;
  int t_0 = 10;
  int iters = 50;

  grid_t *grid = create_grid(l);
  result_t *results = simulate(grid, gamma_inv, beta, d_0, t_0, iters);
  plot_results(results, iters);

  free(results);
  destroy_grid(grid);
}
