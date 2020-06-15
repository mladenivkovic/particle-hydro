/* Cell and grid related stuff */

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "cell.h"
#include "defines.h"
#include "gas.h"
#include "params.h"
#include "utils.h"




#if NDIM == 1
extern cell *grid;
#elif NDIM == 2
extern cell **grid;
#endif

extern params pars;





void cell_init_cell(cell* c){
  /*---------------------------------------*/
  /* Initialize/reset the values of a cell */
  /*---------------------------------------*/

  c->id = 0;
  c->x = 0;
  c->y = 0;
  
}





void cell_init_grid(){
  /*----------------------------------------------------
   * Initialize the grid: Allocate memory for the cells,
   * initialize them, then distribute their position.
   * Convention: 
   *    grid[0][0]       is lower left corner
   *    grid[nx-1][0] is lower right corner
   *    grid[0][nx-1] is upper left corner
   *----------------------------------------------------*/

  log_extra("Initializing grid; ndim=%d, nx=%d", NDIM, pars.nx);

#if NDIM == 1
  
  grid = malloc(pars.nx * sizeof(cell));
  
  for (int i = 0; i < pars.nx; i++){
    cell_init_cell(&grid[i]);
    grid[i].x = (i+0.5)*pars.dx;
    grid[i].id = i;
  }

#elif NDIM == 2
  
  grid = malloc(pars.nx * sizeof(cell*));
  
  for (int i = 0; i < pars.nx; i++){
    grid[i] = malloc(pars.nx * sizeof(cell));
    for (int j = 0; j < pars.nx; j++){
      cell_init_cell(&grid[i][j]);
    }
  }

  for (int j = 0; j < pars.nx; j++){
    for (int i = 0; i < pars.nx; i++){
      grid[i][j].x = (i+0.5)*pars.dx;
      grid[i][j].y = (j+0.5)*pars.dx;
      grid[i][j].id = i + j*pars.nx;
    }
  }
#endif
}







void cell_get_ij(cell* c, int* i, int* j){
  /* ------------------------------------------------ */
  /* Compute the i and j value of a cell such that */
  /* it can be addressed in the grid[] array */
  /* ------------------------------------------------ */

#if NDIM == 1

  *i = c->id;
  *j = 0;

#elif NDIM == 2

  *j = c->id / (pars.nx+2*2);
  *i = c->id - *j * (pars.nx+2*2);

#endif
}
