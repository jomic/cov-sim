#pragma once

#include <set>
#include <vector>
#include <string>

/*
  Returns a list of strings split by the delimiter
*/
std::vector<std::string> split (std::string s, std::string delimiter);

/*
  Returns n unique random integers between 0 and (excluding) max
*/
std::set<int> unique_random_numbers(int n, int max);
