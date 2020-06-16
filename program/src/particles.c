/* particle related routines */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */


#include "gas.h"
#include "params.h"
#include "particles.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>



extern params pars;
extern part* particles;


void init_part_array(){
  /* --------------------------------------
   * Initialize particle array
   * -------------------------------------- */

  log_extra("Initializing particle array");

  if (pars.npart == 0){
    throw_error("Can't allocate space for particle array: Have npart = 0");
  }

  particles = malloc(pars.npart * sizeof(part));

  for (int p = 0; p < pars.npart; p++){
    init_part(&particles[p]);
    particles[p].id = p + 1;
  }
}





void init_part(part* p){
  /* ----------------------------------------
   * Reset all particle values for particle p
   * DOES NOT RESET PARTICLE ID
   * ---------------------------------------- */

  p->x[0] = 0.;
  p->x[1] = 0.;
  p->v[0] = 0.;
  p->v[1] = 0.;
  p->m = 0.;
  p->h = 0.;

  gas_init_pstate(&(p->prim));
  gas_init_cstate(&(p->cons));
}







void part_print_all(void){
  /* ------------------------------------------------
   * Print all particles by increasing particle index
   * ------------------------------------------------ */

  part_print_range(0, pars.npart-1);

}






void part_print_range(int start, int stop){
  /* -------------------------------------------------
   * Print particles with indices starting at start
   * and ending at stop, endpoint INCLUDED.
   *
   * stop must be > start
   * ------------------------------------------------- */

  if (stop < start){
    throw_error("In part_print_range: Can't have stop < start. Vals you gave me were %d, %d", start, stop);
  }


  part_print_header();
  for (int i = start; i <= stop; i++){
    if (i >= pars.npart){
      throw_error("In part_print_range: Got particle index i=%d > npart=%d. start=%d, stop=%d", i,pars.npart, start, stop);
    }
    part p = particles[i];
    part_print_properties(&p);
  }
}






void part_print_properties(part* p){
  /* ----------------------------------------------------
   * Print a single particle's properties
   * ---------------------------------------------------- */

  printf("%5d %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f\n",
      p->id, p->x[0], p->x[1], p->m, p->prim.rho, p->prim.u[0], p->prim.u[1], p->prim.p, p->h);
}





void part_print_header(void){
  /* ----------------------------------------
   * print a table-like header
   * ---------------------------------------- */

  printf("%5s %8s %8s %8s %8s %8s %8s %8s %8s\n",
      "ID", "x", "y", "m", "rho", "u", "v", "p", "h");
}
