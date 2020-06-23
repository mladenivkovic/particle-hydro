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
#include "particles.h"
#include "utils.h"




extern cell *grid;
extern params pars;
extern part* particles;





void cell_init_cell(cell* c){
  /*---------------------------------------
   * Initialize/reset the values of a cell 
   *---------------------------------------*/

  c->id = 0;
  c->x = 0.;
  c->y = 0.;

  c->npic = 0;
  c->cellpartsize = PARTS_ARRAY_SIZE;
  c->cellparts = malloc(c->cellpartsize * sizeof(int));
  
}






void cell_destroy_cell(cell* c){
  /* --------------------------------------
   * Destroy a cell.  In particular, 
   * dealloc the cellparts array.
   * ------------------------------------- */

  free(c->cellparts);
}







void cell_build_grid(){
  /* -------------------------------------------------------
   * Build a useable grid.
   * Criterion for useable is that the number of particles 
   * in a cell and all its direct neighbours is at least
   * some factor of the number of neighbours set by the user
   * ------------------------------------------------------- */

  int nn; /* number of neighbours + 1*/
  int neighs[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; /* indices of neighbours (and this cell) */

  char repeat = 1;
  while (repeat){
    repeat = 0;
    cell_init_grid();
    cell_distribute_particles();

    for (int c = 0; c < pars.ncelltot; c++){
      cell_get_neighbours(&grid[c], neighs, &nn);
      int parts = 0;
      for (int n = 0; n < nn; n++){
        parts += grid[neighs[n]].npic;
      }

      if ((float) parts < CELL_MIN_PARTS_IN_NEIGHBOURHOOD_FACT * pars.nngb){
        /* there are too few particles. We need to rebuild the grid. */
        /* for more particles per cell, reduce number of cells. */
        debugmessage("Cell %d has too few particles around the neighbours.", grid[c].id);
        debugmessage("Expected minimum: %9.3f, I got: %d", CELL_MIN_PARTS_IN_NEIGHBOURHOOD_FACT * pars.nngb, parts);
        debugmessage("Triggering grid rebuild.");
        pars.nx -= 1;
        if (pars.nx <= 0) { throw_error("Grid building: Got nx = 0. :(\n"); }
        repeat = 1;
        break;
      }
    }
  }
}







void cell_init_grid(){
  /*----------------------------------------------------
   * Initialize the grid: Allocate memory for the cells,
   * initialize them, then distribute their position.
   * The "grid" is just a 1D array of cells.
   *----------------------------------------------------*/

  log_extra("Initializing grid");

#if NDIM == 1
  pars.ncelltot = pars.nx;
#elif NDIM == 2
  pars.ncelltot = pars.nx * pars.nx;
#endif
  
  /* update dx here in case we're reiterating*/
  pars.dx = BOXLEN / (float) pars.nx;

  grid = malloc(pars.ncelltot * sizeof(cell));
  
  for (int c = 0; c < pars.ncelltot; c++){
    cell_init_cell(&grid[c]);
    grid[c].id = c;
    int i, j;
    cell_get_ij(&grid[c], &i, &j);
    grid[c].x = ((float) i + 0.5) * pars.dx;
    grid[c].y = ((float) j + 0.5) * pars.dx;
  }

  /* debugging checks and messages */
  if (pars.verbose >= 3){
    debugmessage("  ncells: %4d, nx: %4d, dx: %.3f", pars.ncelltot, pars.nx, pars.dx); 
  }

}







void cell_destroy_grid(){
  /* -------------------------------------
   * dealloc the grid
   * ------------------------------------- */

  log_extra("Deallocating grid");

  for (int i = 0; i < pars.ncelltot; i++){
    cell_destroy_cell(&grid[i]);
  }

  free(grid);

}







void cell_distribute_particles(){
  /* --------------------------------------------
   * Distribute particles into the cells
   * and store them in the particle arrays of the
   * cells
   * -------------------------------------------- */


  log_extra("Distributing particles into cells");

  for (int P = 0; P < pars.npart; P++){
    part p = particles[P];
    
    /* get x/y indices */
    int i = (int) (p.x[0] / (float) pars.dx);
    int j = (int) (p.x[1] / (float) pars.dx);

    /* safety checks */
    if (i >= pars.nx) {printf("In cell_distribute_particles: got i>=nx. p.x=%f\n", p.x[0]);}
    if (j >= pars.nx) {printf("In cell_distribute_particles: got j>=nx. p.y=%f\n", p.x[1]);}

    /* now get cell index  and cell*/
    int ind = j * pars.nx + i;
    cell_add_particle(&grid[ind], P);
  }

  /* debugging notes and checks */
  if (pars.verbose >= 3){
    int npmin = pars.npart;
    int npmax = 0;
    int nptot = 0;
    for (int c = 0; c < pars.ncelltot; c++){
      int np = grid[c].npic;
      if (np < npmin) npmin = np;
      if (np > npmax) npmax = np;
      nptot += np;
    }
    debugmessage("Number of particles in cells:");
    debugmessage("  Min %4d, Max %4d, Tot %4d/%4d, Mean %.3f", 
      npmin, npmax, nptot, pars.npart, (float) nptot / (float) pars.ncelltot);
  }
}






void cell_add_particle(cell* c, int pind){
  /* --------------------------------------------
   * Add a particle index pind to a cell particle
   * array
   * -------------------------------------------- */

  if (c->npic == c->cellpartsize){
    /* we need to make a new array with more space */
    int* newarr = malloc((c->cellpartsize + PARTS_ARRAY_SIZE)*sizeof(int));

    /* copy the data into new array */
    for (int i = 0; i < c->cellpartsize; i++){
      newarr[i] = c->cellparts[i];
    }

    /* now free the old array and switch to the new one */
    free(c->cellparts);
    c->cellparts = newarr;
    c->cellpartsize += PARTS_ARRAY_SIZE;
  }

  /* now actually add the new particle */
  c->cellparts[c->npic] = pind;
  c->npic += 1;
}









void cell_get_neighbours(cell* c, int neighs[9], int* nneighs){
  /* ---------------------------------------------------------
   * Find the indices of all neighbour cells of this cell and 
   * write them (and this cell's index as well) into the neighs
   * array. Write how many entries are in that array in
   * nneighs integer.
   * First entry of neighs[] array is always the cell itself,
   * so you can just loop over the neighs array from 0 to 
   * nneighs
   * --------------------------------------------------------- */

  *nneighs = 0;


#if NDIM == 1

  /* add the cell itself first */
  neighs[0] = c->id;
  *nneighs = 1;

  /* periodic boundary conditions */
  if (pars.boundary == 0) { 

    /* left boundary */
    if (c->id == 0){
      neighs[*nneighs] = pars.nx - 1;
    } else {
      neighs[*nneighs] = c->id - 1;
    }
    *nneighs += 1;

    /* right boundary */
    if (c->id == pars.nx - 1){
      neighs[*nneighs] = 0;
    } else {
      neighs[*nneighs] = c->id + 1;
    }
    *nneighs += 1;
  }

  /* transmissive boundary conditions */
  else if (pars.boundary == 1){
    
    /* left boundary */
    if (c->id > 0){
      neighs[*nneighs] = c->id - 1;
    }
    *nneighs += 1;

    /* right boundary */
    if (c->id < pars.nx - 1){
      neighs[*nneighs] = c->id + 1;
    }
    *nneighs += 1;
  }




#elif NDIM == 2


  /* first get indices in x and y direction. */
  int i, j;
  cell_get_ij(c, &i, &j);


  /* periodic boundary conditions */
  int left, right, top, bottom; /* indices of left, right, top, bottom rows/columns */
  if (pars.boundary == 0) { 

    /* left boundary */
    if (i == 0){
      left = pars.nx - 1;
    } else {
      left = i - 1;
    }

    /* right boundary */
    if (i == pars.nx - 1){
      right = 0;
    } else {
      right = i + 1;
    }

    /* top boundary */
    if (j == pars.nx - 1){
      top = 0;
    } else {
      top = j + 1;
    }

    /* bottom boundary */
    if (j == 0) {
      bottom = pars.nx - 1;
    } else {
      bottom = j - 1;
    }


    /* now add neighbours to array */
    /* always first add cell itself */
    neighs[*nneighs] = cell_get_ind_from_ij(i, j);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(left, bottom);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(left, j);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(left, top);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(i, bottom);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(i, top);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(right, bottom);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(right, j);
    *nneighs += 1;
    neighs[*nneighs] = cell_get_ind_from_ij(right, top);
    *nneighs += 1;

  }

  /* transmissive boundary conditions */
  else if (pars.boundary == 1){

    /* initialize all indices to -1 */
    left = -1;
    right = -1;
    top = -1;
    bottom = -1;


    /* left boundary */
    if (i != 0){
      left = i - 1;
    }

    /* right boundary */
    if (i != pars.nx - 1){
      right = i + 1;
    }

    /* top boundary */
    if (j != pars.nx - 1){
      top = j + 1;
    }

    /* bottom boundary */
    if (j != 0){
      bottom = j - 1;
    }


    /* now add the neighbours if they exist */
    /* always first add cell itself */
    neighs[*nneighs] = cell_get_ind_from_ij(i, j);
    *nneighs += 1;

    if (left != -1){

      neighs[*nneighs] = cell_get_ind_from_ij(left, j);
      *nneighs += 1;

      if (bottom != -1){
        neighs[*nneighs] = cell_get_ind_from_ij(left, bottom);
        *nneighs += 1;
      }

      if (top != -1){
        neighs[*nneighs] = cell_get_ind_from_ij(left, top);
        *nneighs += 1;
      }
    }

    if (bottom != -1){
      neighs[*nneighs] = cell_get_ind_from_ij(i, bottom);
      *nneighs += 1;
    }

    if (top != -1){
      neighs[*nneighs] = cell_get_ind_from_ij(i, top);
      *nneighs += 1;
    }

    if (right != -1){

      neighs[*nneighs] = cell_get_ind_from_ij(right, j);
      *nneighs += 1;

      if (bottom != -1){
        neighs[*nneighs] = cell_get_ind_from_ij(right, bottom);
        *nneighs += 1;
      }

      if (top != -1){
        neighs[*nneighs] = cell_get_ind_from_ij(right, top);
        *nneighs += 1;
      }

    }
  }

#endif

}






int cell_get_ind_from_ij(int i, int j){
  /* --------------------------------------------
   * Compute cell index in grid from given i, j
   * -------------------------------------------- */

  return(j*pars.nx + i);
}








void cell_get_ij(cell* c, int* i, int* j){
  /* ------------------------------------------------
   * Compute the i and j value of a cell such that
   * it can be addressed in the grid[] array
   * ------------------------------------------------ */

#if NDIM == 1
  *i = c->id;
  *j = 0;
#elif NDIM == 2
  *i = c->id % pars.nx;
  *j = c->id / pars.nx;
#endif
}





void cell_print_grid_layout(){
  /* --------------------------------------------
   * print a cell grid layout for checks on
   * screen
   * -------------------------------------------- */

  printf("GRID LAYOUT\n");

#if NDIM == 1
  printf("|");
  for (int i = 0; i < pars.nx; i ++){
    printf("%3d |", grid[i].id);
  }
  printf("\n");
#elif NDIM == 2
  for (int j = pars.nx - 1; j >=0; j--){
    /* print horizontal line */
    for (int i = 0; i < (pars.nx * 5) + 1; i++){ printf("-");}
    printf("\n");

    printf("|");
    for (int i = 0; i < pars.nx; i++){
      printf("%3d |", grid[cell_get_ind_from_ij(i,j)].id);
    }
    printf("\n");
  }
  /* print horizontal line */
  for (int i = 0; i < (pars.nx * 5) + 1; i++){ printf("-");}
  printf("\n");
#endif

}
