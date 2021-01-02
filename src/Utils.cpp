#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "Utils.hpp"
using namespace std;

template<typename T>
string to_string(set<T> set_) {
  string to_string = "< ";
  for(T elem : set_) { to_string += std::to_string(elem) + " "; }
  to_string += ">";
  return to_string;
}

set<int> unique_random_numbers(int count, int max) {
  set<int> numbers;
  if (count > max) {
    cout << "random_numbers: count cannot be larger than total" << endl;
    return numbers;
  }
  while ((int)numbers.size() < count) { numbers.insert(rand() % max); }
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
