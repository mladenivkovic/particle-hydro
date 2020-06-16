/* particle structs and routines */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */



#ifndef PARTICLE_H
#define PARTICLE_H

#include "gas.h"

/* particle struct */
typedef struct {

  int id;     /* particle ID */

  float x[2];   /* particle position */
  float v[2];   /* particle velocity */
  float m;      /* particle mass */ 
  float h;      /* particle smoothing length */


  pstate prim;  /* primitive fluid state */
  cstate cons;  /* conserved fluid state */

} part;




void init_part_array(void);
void init_part(part* p);


/* particle STDOUT printing */
void part_print_all(void);
void part_print_range(int start, int stop);
void part_print_properties(part* p);
void part_print_header(void);

#endif