/* Cell and grid related stuff */

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef CELL_H
#define CELL_H


#include "gas.h"
#include "defines.h"

typedef struct{

  int id;

  float x;          /* cell center coordinates */
  float y;          /* cell center coordinates */

  int npic;         /* number of particles in this cell */
  int* cellparts;   /* array particles in this cell */
  int cellpartsize; /* total cellparts array size, how many elements fit in */

} cell;




void cell_init_cell(cell* c);
void cell_destroy_cell(cell* c);

void cell_build_grid(); /* this one actually builds grid and calls init_grid */
void cell_init_grid();
void cell_destroy_grid();

void cell_distribute_particles();
void cell_add_particle(cell* c, int pind);

void cell_get_neighbours(cell* c, int neighs[9], int* nneighs);
int cell_get_ind_from_ij(int i, int j);
void cell_get_ij(cell* c, int* i, int* j);

void cell_print_grid_layout();
#endif
