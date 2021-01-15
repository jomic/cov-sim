#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>
#include "Utils.hpp"
using namespace std;

set<int> unique_random_numbers(int count, int max) {
  set<int> numbers;
  if (count > max) {
    cout << "random_numbers: count cannot be larger than total" << endl;
    return numbers;
  }
  
  while ((int)numbers.size() < count) {
    int number = sample_nonnegative(max);
    numbers.insert(number);
  }
  return numbers;
}

// https://stackoverflow.com/a/46931770
vector<string> split (string strng, string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;

  while ((pos_end = strng.find (delimiter, pos_start)) != string::npos) {
    token = strng.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (strng.substr (pos_start));
  return res;
}

int sum(vector<int> vctr) {
  int sum = 0;
  for(int i : vctr) { sum += i; }
  return sum;
}

default_random_engine generator;
void seed_generator(int seed) {
  generator.seed(seed);
}

bool sample_bernoulli(float p) {
  bernoulli_distribution distribution(p);
  return distribution(generator);
}

int sample_binomial(int n, float p) {
  binomial_distribution<int> distribution(n, p);
  return distribution(generator);
}

int sample_uniform_int(int a, int b) {
  uniform_int_distribution<int> distribution(a, b);
  return distribution(generator);
}

int sample_nonnegative(int max) {
  if (max <= 0)
    return 0;
  else
    return sample_uniform_int(0, max - 1);
}
