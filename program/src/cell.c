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





void cell_init_cell(cell * c){
  /*---------------------------------------*/
  /* Initialize/reset the values of a cell */
  /*---------------------------------------*/

  c->id = 0;
  c->x = 0;
  c->y = 0;
  
  gas_init_pstate(&(c->prim));
  gas_init_pstate(&(c->pflux));
  gas_init_cstate(&(c->cons));
  gas_init_cstate(&(c->cflux));

  c->acc[0] = 0.;
  c->acc[1] = 0.;
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




void cell_set_boundary(){
  /* -------------------------------------------------------
   * enforce boundary conditions. 
   * This function only picks out the pairs of real
   * and ghost cells in a row or column and then 
   * calls the function that actually copies the data.
   * ------------------------------------------------------- */

  debugmessage("Setting boundary conditions");

  /* TODO: this entire function is junk. Remove/adapt properly. */
  cell* realL[2];
  cell* realR[2];
  cell* ghostL[2];
  cell* ghostR[2];

#if NDIM == 1

  for(int i = 0; i < 2; i++){
    realL[i] = &(grid[2+i]);
    realR[i] = &(grid[pars.nx+i]); /* = last index of a real cell  - BC + (i + 1) */
    ghostL[i] = &(grid[i]);
    ghostR[i] = &(grid[pars.nx+2+i]);
  }
  cell_real_to_ghost(realL, realR, ghostL, ghostR, /*dimension=*/0);

#elif NDIM == 2

  /* first do all left-right boundaries */
  for (int j = 0; j < pars.nx + 4; j++){
    for(int i = 0; i < 2; i++){
      realL[i] = &(grid[2+i][j]);
      realR[i] = &(grid[pars.nx+i][j]); /* = last index of a real cell  - BC + (i + 1) */
      ghostL[i] = &(grid[i][j]);
      ghostR[i] = &(grid[pars.nx+2+i][j]);
    }
    cell_real_to_ghost(realL, realR, ghostL, ghostR, /*dimension=*/0);
  }

  /* now do all upper-lower boundaries */
  for (int i = 0; i < pars.nx + 4; i++){
    for(int j = 0; j < 2; j++){
      realL[j] = &(grid[i][2+j]);
      realR[j] = &(grid[i][pars.nx+j]); /* = last index of a real cell  - BC + (j + 1) */
      ghostL[j] = &(grid[i][j]);
      ghostR[j] = &(grid[i][pars.nx+2+j]);
    }
    cell_real_to_ghost(realL, realR, ghostL, ghostR, /*dimension=*/1);
  }

#endif
}





void cell_real_to_ghost(cell** realL, cell** realR, cell** ghostL, cell** ghostR, int dimension){
  /* ------------------------------------------------------------
   * apply the boundary conditions from real to ghost cells
   *
   * realL:     array of pointers to real cells with lowest index
   * realR:     array of pointers to real cells with highest index
   * ghostL:    array of pointers to ghost cells with lowest index
   * ghostR:    array of pointers to ghost cells with highest index
   * dimension: dimension integer. 0 for x, 1 for y. Needed for
   *            reflective boundary conditions.
   *
   * all arguments are arrays of size BC, defined in defines.h
   * lowest array index is also lowest index of cell in grid
   * ------------------------------------------------------------ */

  if (pars.boundary == 0){
    /* periodic boundary conditions */
    /* ---------------------------- */
    for (int i = 0; i < 2; i++){
      cell_copy_boundary_data(realL[i], ghostR[i]);
      cell_copy_boundary_data(realR[i], ghostL[i]);
    }


  } else if (pars.boundary == 1){
    /* reflective boundary conditions */
    /* ------------------------------ */
    for (int i = 0; i < 2; i++){
      cell_copy_boundary_data_reflective(realL[i], ghostL[2-1-i], dimension);
      cell_copy_boundary_data_reflective(realR[i], ghostR[2-1-i], dimension);
    }


  } else if (pars.boundary == 2){
    /* transmissive boundary conditions */
    /* -------------------------------- */
    for (int i = 0; i < 2; i++){
      cell_copy_boundary_data(realL[i], ghostL[i]);
      cell_copy_boundary_data(realR[2-1-i], ghostR[i]);
    }
  }
}







void cell_copy_boundary_data(cell* real, cell* ghost){
  /* ---------------------------------------------------------
   * Copies the actual data needed for boundaries from a real 
   * cell to a ghost cell
   * --------------------------------------------------------- */

  ghost->prim.rho = real->prim.rho;
  ghost->prim.u[0] = real->prim.u[0];
  ghost->prim.u[1] = real->prim.u[1];
  ghost->prim.p = real->prim.p;

  ghost->cons.rho = real->cons.rho;
  ghost->cons.rhou[0] = real->cons.rhou[0];
  ghost->cons.rhou[1] = real->cons.rhou[1];
  ghost->cons.E = real->cons.E;
}






void cell_copy_boundary_data_reflective(cell* real, cell* ghost, int dimension){
  /* ---------------------------------------------------------
   * Copies the actual data needed for boundaries from a real 
   * cell to a ghost cell. Here for a reflective boundary 
   * condition, where we need to invert the velocities.
   *
   * cell* real: pointer to real cell from which we take data
   * cell* ghost: pointer to ghost cell into which we copy data
   * int dimension: in which dimension the reflection is supposed to be
   * --------------------------------------------------------- */

  ghost->prim.rho = real->prim.rho;
  ghost->prim.u[dimension] = - real->prim.u[dimension];
  ghost->prim.u[(dimension + 1) % 2] = real->prim.u[(dimension + 1) % 2];
  ghost->prim.p = real->prim.p;

  ghost->cons.rho = real->cons.rho;
  ghost->cons.rhou[dimension] = - real->cons.rhou[dimension];
  ghost->cons.rhou[(dimension + 1) % 2] = real->cons.rhou[(dimension + 1) % 2];
  ghost->cons.E = real->cons.E;
}





void cell_reset_fluxes(){
  /* ----------------------------------------------------
   * reset the fluxes to zero in all cells in the grid
   * ---------------------------------------------------- */

  debugmessage("Resetting fluxes to zero.");

#if NDIM == 1
  for (int i = 0; i < pars.nx; i++){
    gas_init_pstate(&(grid[i].pflux));
    gas_init_cstate(&(grid[i].cflux));
  }
#elif NDIM == 2
  for (int i = 0; i < pars.nx; i++){
    for (int j = 0; j < pars.nx; j++){
      gas_init_pstate(&(grid[i][j].pflux));
      gas_init_cstate(&(grid[i][j].cflux));
    }
  }

#endif
}





void cell_get_pstates_from_cstates(){
  /* ---------------------------------------------
   * Computes the primitive state from conserved
   * states for all cells 
   * --------------------------------------------- */

#if NDIM == 1
  for (int i = 0; i < pars.nx; i++){
    cell* c = &grid[i];
    gas_cons_to_prim(&c->cons, &c->prim);
  }
#elif NDIM == 2
  for (int i = 0; i < pars.nx ; i++){
    for (int j = 0; j < pars.nx; j++){
      cell* c = &grid[i][j];
      gas_cons_to_prim(&c->cons, &c->prim);
    }
  }
#endif
}





void cell_get_cstates_from_pstates(){
  /* ---------------------------------------------
   * Computes the conserve state from primitive
   * states for all cells 
   * --------------------------------------------- */

#if NDIM == 1
  for (int i = 0; i < pars.nx; i++){
    cell* c = &grid[i];
    gas_prim_to_cons(&c->prim, &c->cons);
  }
#elif NDIM == 2
  for (int i = 0; i < pars.nx ; i++){
    for (int j = 0; j < pars.nx; j++){
      cell* c = &grid[i][j];
      gas_prim_to_cons(&c->prim, &c->cons);
    }
  }
#endif
}





float cell_get_total_mass(){
  /* -----------------------------------
   * Compute the total "mass" currently
   * on the grid.
   * ----------------------------------- */

  float mtot = 0;

#if NDIM == 1
  for (int i = 0; i < pars.nx; i++){
    mtot += grid[i].prim.rho;
  }
  mtot *= pars.dx;
  return(mtot);
#elif NDIM == 2
  for (int i = 0; i < pars.nx; i++){
    for (int j = 0; j < pars.nx; j++){
      mtot += grid[i][j].prim.rho;
    }
  }
  mtot *= pars.dx * pars.dx;
  return(mtot);
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
