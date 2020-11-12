#pragma once
#include <stdint.h>

using edge_t = uint_least16_t;
using restriction_t = edge_t;

enum class contact_type : edge_t {
	daycare = 0x1,
	school = 0x2,
	upper_school = 0x4,
	university = 0x8,

	dist_work = 0x10,
	no_dist_work = 0x20,
	customer_serv = 0x40,
	commute = 0x80,

	sports = 0x100,
	concerts = 0x200,
	restaurants = 0x400,
	bars = 0x800,

	friends = 0x1000,
	family = 0x2000,
	passerby = 0x4000
};

float get_probability();

void set_probability(float prob);

edge_t get_empty_edge();

restriction_t get_empty_restriction();

int contact_type_count(edge_t *e);

void add_contact_type(edge_t *e, contact_type c);

void remove_contact_type(edge_t *e, contact_type c);

void add_restriction(restriction_t *r, contact_type c);

float infection_probability(edge_t *e);

float infection_probability(edge_t *e, restriction_t *r);