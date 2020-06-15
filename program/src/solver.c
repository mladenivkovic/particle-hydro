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







void solver_get_hydro_dt(float* dt){
  /* ---------------------------------------------- 
   * Computes the maximal allowable time step size
   * find max velocity present, then apply Courant
   * number to reduce it.
   * ---------------------------------------------- */

  debugmessage("Called solver_get_hydro_dt", *dt);

  *dt = 1e-6;

}







void solver_advance_step_hydro(float* dt, int dimension){
  /* ---------------------------------------------
   * Integrate the equations for one time step
   * --------------------------------------------- */

  debugmessage("Called solver_advance_step with dt = %f", *dt);

}
