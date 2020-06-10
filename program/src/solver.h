/* top level file for the hydro method solver */


/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef SOLVER_H
#define SOLVER_H

#include "defines.h"


#if SOLVER == SPH-DS
#include "solver/SPH-density-entropy.h"
#elif SOLVER == MESHLESS
#include "solver/meshless.h"
#elif SOLVER == MESHLESS_IVANOVA
#include "solver/meshless.h"
#endif


void solver_step(float* t, float* dt, int step,  int* write_output);


void solver_get_hydro_dt(float* dt, int step);
void solver_advance_step_hydro(float* dt, int dimension);
void solver_update_state_hydro(cell* left, cell* right, float dtdx);

#ifdef ADVECTION_KEEP_VELOCITY_CONSTANT
void solver_advection_check_global_velocity();
#endif

#endif
