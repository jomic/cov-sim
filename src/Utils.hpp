#pragma once
#include <set>
#include <string>
#include <vector>
using namespace std;

/**
  Returns a list of strings split by the delimiter
*/
vector<string> split (string strng, string delimiter);

/**
  Returns n unique random integers between 0 and (excluding) max
*/
set<int> unique_random_numbers(int numbr, int max);

/**
   Sum all the values of an integer vector
 */
int sum(vector<int> vctr);

/**
   Sample the generator used for random samples
 */
void seed_generator(int seed);

/**
   Sample a bernoulli distribution with success probability p
 */
bool sample_bernoulli(float p);

/**
   Sample a binomial distribution with n attempts of probability p 
 */
int sample_binomial(int n, float p);

/**
   Sample an integet in the uniform range [a, b]
 */
int sample_uniform_int(int a, int b);

/**
   Sample an integer in the uniform range [0, max)
 */
int sample_nonnegative(int max);
