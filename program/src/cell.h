/* Cell and grid related stuff */

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef CELL_H
#define CELL_H


#include "gas.h"
#include "defines.h"

typedef struct{

  int id;

  float x;        /* x position of cell center */
  float y;        /* y position of cell center */

} cell;



#if NDIM == 1
extern cell* grid;
#elif NDIM == 2
extern cell** grid;
#endif


void cell_init_cell(cell* c);
void cell_init_grid();
void cell_set_boundary();

void cell_get_ij(cell* c, int* i, int* j);

#endif
