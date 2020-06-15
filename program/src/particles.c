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
