#include <math.h>
#include "edges.h"

float g_prob = 0.01f;

float get_probability() {
	return g_prob;
}

void set_probability(float prob) {
	g_prob = prob;
}

edge_t get_empty_edge() {
	return edge_t{ 0 };
}

restriction_t get_empty_restriction() {
	restriction_t r{ 0 };
	return ~r;
}

int contact_type_count(edge_t *e) {
	// "Without specifying an appropriate "-march" on the 
	// command line gcc generates a call to the __popcountdi2
	// function rather than the popcnt instruction. See: https://godbolt.org/z/z1BihM"
	return __builtin_popcount(*e);
}

void add_contact_type(edge_t *e, contact_type c) {
	*e |= static_cast<edge_t>(c);
}

void remove_contact_type(edge_t *e, contact_type c) {
	*e &= ~static_cast<edge_t>(c);
}

void add_restriction(restriction_t *r, contact_type c) {
	*r &= ~static_cast<edge_t>(c);
}

void remove_restriction(restriction_t *r, contact_type c) {
	*r |= static_cast<edge_t>(c);
}

restriction_t combine_restrictions(restriction_t *a, restriction_t *b) {
	restriction_t r = *a & *b;
	return r;
}

float infection_probability(edge_t *e) {
	int n = contact_type_count(e);
	float prob = 1 - pow(1 - g_prob, n);
	return prob;
}

float infection_probability(edge_t *e, restriction_t *r) {
	edge_t restricted_e = *e & *r;
	int n = contact_type_count(r);
	float prob = 1 - pow(1 - g_prob, n);
	return prob;
}
