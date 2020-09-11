/* particle structs and routines */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "gas.h"

/* particle struct */
typedef struct {

  int id; /* particle ID */

  float x[2]; /* particle position */
  float v[2]; /* particle velocity */
  float m;    /* particle mass */
  float h;    /* particle smoothing length */

  pstate prim; /* primitive fluid state */
  cstate cons; /* conserved fluid state */

  int *neigh_iact; /* neighbours we interact with */
  int nneigh_iact; /* number of neighbours to interact with; = sizes of
                      neigh_iact and r arrays */
  float *r;        /* distances to neighbours to interact with */

} part;

void init_part_array(void);
void init_part(part *p);
void free_part_arrays();

void part_get_smoothing_lengths(); /* compute all smoothing lengths */
void part_compute_h(
    part *p, float *r, int *neighs,
    int nneigh); /* compute smoothing length of given particle */

/* particle STDOUT printing */
void part_print_all(void);
void part_print_range(int start, int stop);
void part_print_properties(part *p);
void part_print_header(void);

/* other particle printing routines */
void part_write_smoothing_lengths(int step);

#endif
