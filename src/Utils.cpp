#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "Utils.hpp"
using namespace std;

set<int> unique_random_numbers(int numbr, int max) {
  set<int> numbers;
  if (numbr > max) {
    cout << "random_numbers: numbr cannot be larger than total" << endl;
    return numbers;
  }
  while ((int)numbers.size() < numbr) {
      numbers.insert(rand() % max);
    }
  return numbers;
}

// https://stackoverflow.com/a/46931770
vector<string> split (string a_string, string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;

  while ((pos_end = a_string.find (delimiter, pos_start)) != string::npos) {
    token = a_string.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (a_string.substr (pos_start));
  return res;
}
