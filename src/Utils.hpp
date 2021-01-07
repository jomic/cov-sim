#pragma once
#include <set>
#include <string>
#include <vector>

/**
  Returns a list of strings split by the delimiter
*/
std::vector<std::string> split (std::string strng, std::string delimiter);

/**
  Returns n unique random integers between 0 and (excluding) max
*/
std::set<int> unique_random_numbers(int numbr, int max);
