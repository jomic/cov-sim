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

/*
	Get the probability of an infected person infecting a susceptible person
	within a single time step, if they have exactly one contact type.
*/
float get_probability();

/*
	Set the probability of an infected person infecting a susceptible person
	within a single time step, if they have exactly one contact type.
*/
void set_probability(float prob);

/*
	Get an edge without any contact types.
*/
edge_t get_empty_edge();

/*
	Get a restriction without any contact types restricted.
*/
restriction_t get_empty_restriction();

/*
	Get the total number of contact types on an edge.
*/
int contact_type_count(edge_t *e);

/*
	Add a contact type to an edge.
*/
void add_contact_type(edge_t *e, contact_type c);

/*
	Remove a contact type from an edge.
*/
void remove_contact_type(edge_t *e, contact_type c);

/*
	Add a contact type to a restriction.
*/
void add_restriction(restriction_t *r, contact_type c);

/*
	Remove a contact type from a restriction.
*/
void remove_restriction(restriction_t *r, contact_type c);

/*
	Combine two existing restrictions into a single one.
*/
restriction_t combine_restrictions(restriction_t *a, restriction_t *b);

/*
	Get the probability of an edge transmitting an infection with every
	contact type on the edge taken into account.
*/
float infection_probability(edge_t *e);

/*
	Get the probability of an edge transmitting an infection with some 
	contact types prevented with a restriction.
*/
float infection_probability(edge_t *e, restriction_t *r);