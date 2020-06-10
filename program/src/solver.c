/* some method independent routines *
 * Written by Mladen Ivkovic, MAR 2020
 * mladen.ivkovic@hotmail.com           */

#include <math.h>

#include "cell.h"
#include "io.h"
#include "params.h"
#include "solver.h"
#include "utils.h"

#if NDIM == 1
extern cell *grid;
#elif NDIM == 2
extern cell **grid;
#endif

extern params pars;







void solver_get_hydro_dt(float* dt, int step){
  /* ---------------------------------------------- 
   * Computes the maximal allowable time step size
   * find max velocity present, then apply Courant
   * number to reduce it.
   * ---------------------------------------------- */

  debugmessage("Called solver_get_hydro_dt", *dt);


#if NDIM == 1
  float umax = 0.;

  for (int i = 0; i < pars.nx; i++){
    float uxabs = fabs(grid[i].prim.u[0]);
    float a = gas_soundspeed(&grid[i].prim);
    float S = uxabs + a;
    if (S > umax){ umax = S; }
  }

  *dt = pars.ccfl * pars.dx / umax;

#elif NDIM == 2

  float uxmax = 0;
  float uymax = 0;

  for (int i = 0; i < pars.nx; i++){
    for (int j = 0; j < pars.nx; j++){
      float uxabs = fabs(grid[i][j].prim.u[0]);
      float a = gas_soundspeed(&grid[i][j].prim);
      float S = uxabs + a;
      if (S > uxmax){ uxmax = S; }
      float uyabs = fabs(grid[i][j].prim.u[1]);
      S = uyabs + a;
      if (S > uymax){ uymax = S; }
    }
  }

  float uxdx = uxmax / pars.dx; /* ux_max / dx */
  float uydy = uymax / pars.dx; /* uy_max / dy */
  
  *dt = pars.ccfl / ( uxdx + uydy );

#endif /* NDIM == 2*/

  if (pars.force_dt > 0){
    if (*dt > pars.force_dt){
      *dt = pars.force_dt;
    } else{
      throw_error("I require a smaller timestep dt=%g than force_dt=%g is demanding.",
        *dt, pars.force_dt);
    }
  }

  /* sometimes there might be trouble with sharp discontinuities at the beginning, 
   * so reduce the timestep for the first few steps */
  if (step <=5) *dt *= 0.2;

  /* safety check */
  if (*dt <= DT_MIN) throw_error("Got weird time step? dt=%12.4e", *dt);
}







void solver_advance_step_hydro(float* dt, int dimension){
  /* ---------------------------------------------
   * Integrate the equations for one time step
   * --------------------------------------------- */

  debugmessage("Called solver_advance_step with dt = %f", *dt);

  float dtdx = *dt / pars.dx;

#if NDIM == 1
  for (int i = 0; i < pars.nx; i++){
    solver_update_state_hydro(&(grid[i-1]), &(grid[i]), dtdx);
  }
#elif NDIM == 2
  for (int i = 0; i < pars.nx; i++){
    for (int j = 0; j < pars.nx; j++){
      if (dimension == 0){
        solver_update_state_hydro(&(grid[i-1][j]), &grid[i][j], dtdx);
      } else if (dimension == 1){
        solver_update_state_hydro(&(grid[i][j-1]), &grid[i][j], dtdx);
      }
    }
  }
#endif
}





void solver_update_state_hydro(cell* left, cell* right, float dtdx){
  /* ------------------------------------------------------
   * Update the state using the fluxes in the cell and dt
   * dtdx: dt / dx
   * right is the cell with index i that we are trying to
   * update; left is the cell i-1, which stores the flux
   * at i-1/2
   * ------------------------------------------------------ */

  right->cons.rho     = right->cons.rho     + dtdx * (left->cflux.rho     - right->cflux.rho );
  right->cons.rhou[0] = right->cons.rhou[0] + dtdx * (left->cflux.rhou[0] - right->cflux.rhou[0] );
  right->cons.rhou[1] = right->cons.rhou[1] + dtdx * (left->cflux.rhou[1] - right->cflux.rhou[1] );
  right->cons.E       = right->cons.E       + dtdx * (left->cflux.E       - right->cflux.E );
}
