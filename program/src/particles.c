/* particle related routines */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */


#include "cell.h"
#include "defines.h"
#include "gas.h"
#include "kernel.h"
#include "params.h"
#include "particles.h"
#include "sort.h"
#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>



extern params pars;
extern part* particles;
extern cell* grid;


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

  p->neigh_iact = NULL;
  p->nneigh_iact = 0;
  p->r = NULL;
}




void free_part_arrays(){
  /*----------------------------------
   * Deallocate arrays that are stored
   * in particle structs
   * --------------------------------- */

  for (int i = 0; i < pars.npart; i++){
    free(particles[i].neigh_iact);
    free(particles[i].r);
  }
}







void part_get_smoothing_lengths(){
  /* -------------------------------------------------
   * Determine the smoothing length and the neighbours
   * to interact with for all particles
   *-------------------------------------------------- */


  /* Loop over all cells. Find neighbour cells for each cell, 
   * then build particle neighbour lists based on cell particle
   * lists */

  int neighs[9];  /* cell neighbour array */
  int nn;         /* number of cell neighbours */
  int npctot;     /* total number of particles in cells + neighbours */
  int* allneighs; /* all neighbour candidates of particles in a cell */
  float* r;       /* distances of all neighbour candidates of particles in a cell */
  float* x;       /* x coordinate of all neighbour candidates of particles in a cell */
  float* y;       /* y coordinate of all neighbour candidates of particles in a cell */

  for (int c = 0; c < pars.ncelltot; c++){
    /* get neighbours */
    cell_get_neighbours(&grid[c], neighs, &nn);

    /* how many particles are we dealing with here? */
    npctot = 0;
    for (int n = 0; n < nn; n++){
      npctot += grid[neighs[n]].npic; 
    }

    /* allocate particle neighbour arrays */
    allneighs = malloc(npctot * sizeof(int));
    r = malloc(npctot * sizeof(float));
    x = malloc(npctot * sizeof(float));
    y = malloc(npctot * sizeof(float));

  
    /* fill up arrays */
    int f = 0;
    for (int n = 0; n < nn; n++){
      cell C = grid[neighs[n]];
      for (int np = 0; np < C.npic; np++){
        allneighs[f] = C.cellparts[np];
        x[f] = particles[allneighs[f]].x[0];
        y[f] = particles[allneighs[f]].x[1];
        f += 1;
      }
    }


    /* Now loop over all particles of this cell */
    for (int np = 0; np < grid[c].npic; np++){
      int pind = grid[c].cellparts[np];
      /* get particle distances w.r.t. this particle*/
      for (int n = 0; n < npctot; n++){
        float dx = x[n] - particles[pind].x[0];
        float dy = y[n] - particles[pind].x[1];
        if (pars.boundary == 0){
          /* add periodicity corrections */
          if (dx >  0.5 * BOXLEN) dx -= BOXLEN;
          if (dx < -0.5 * BOXLEN) dx += BOXLEN;
          if (dy >  0.5 * BOXLEN) dy -= BOXLEN;
          if (dy < -0.5 * BOXLEN) dy += BOXLEN;
        }
        r[n] = sqrtf(dx*dx + dy*dy);
      }
      part_compute_h(&particles[pind], r, allneighs, npctot);
    }


    /* free arrays for this cell */
    free(allneighs);
    free(r);
    free(x);
    free(y);
  }
}







void part_compute_h(part* p, float* r, int* neigh, int nneigh){
  /* ----------------------------------------------------------------
   * Iteratively compute the smoothing length for given particle p.
   * r:   distances to all neighbouring particles
   * neigh: array of neighbour particle indices
   * nneigh: number of elements in neigh array
   * ---------------------------------------------------------------- */

  /* create copies of neigh array so you can safely play with them */
  int* neighcpy = malloc(nneigh * sizeof(int));
  for (int i = 0; i < nneigh; i++){
    neighcpy[i] = neigh[i];
  }


  /* sort neighcpy and r array by increasing r */
  quicksort_float_int_follower(r, neighcpy, nneigh);


  /* take initial guess for compact support radius for this particle */
  float Hi = r[(int) (pars.nngb + 0.5)];
  float rhoi; /* density of this particle */

  int niter = 0;

#if NDIM == 1
  float eta = pars.nngb * 0.5 / KERNEL_Hoverh;
  float mipower = p->m; /* m_i ^ {1 / \nu} */
#elif NDIM == 2
  float eta = sqrtf(pars.nngb / PI) / KERNEL_Hoverh;
  float mipower = sqrtf(p->m); /* m_i ^ {1 / \nu} */
#endif


  while (niter < ITER_MAX_H){
    niter += 1;

    rhoi = 0.;
    float dfdh_sum = 0; /* the summation part in df/dh */
    float hi = kernel_hfromH(Hi);

    /* do neighbour loop */
    for (int i = 0; r[i] <= Hi; i++){
      part pj = particles[neighcpy[i]];
      float W = kernel_W(r[i], hi);
      float dWdr = kernel_dWdr(r[i], hi);
      rhoi += pj.m * W;
      dfdh_sum += pj.m * ( W + r[i]/NDIM *dWdr );
      if (i == nneigh - 1) break; /* safety measure */
    }

    /* now compute f and df/dh */
    float rhopower = pow(rhoi, -1. - 1./NDIM); /* rho_i ^{-1 - 1/\nu} */
    float f = hi - eta * mipower * rhopower * rhoi;
    float dfdh = 1. - eta / hi * mipower * rhopower * dfdh_sum;

    float Hinew = Hi - f / dfdh;
    if (fabs(Hinew - Hi) < EPSILON_H * Hi){
      Hi = Hinew;
      break;
    } else {
      Hi = Hinew;
    }

  }

  if (niter == ITER_MAX_H){
    throw_error("reached max number of iterations for smoothing length of particle %d", p->id);
  } 


  /* store results! */
  float hi = kernel_hfromH(Hi);
  p->h = hi;
  p->prim.rho = rhoi;
  
  /* now get neighbours array size */
  p->nneigh_iact = 0;
  for (int i = 0; r[i] <= Hi; i++){
    p->nneigh_iact += 1;
  }

  /* allocate exact size arrays */
  p->neigh_iact = malloc(p->nneigh_iact * sizeof(int));
  p->r = malloc(p->nneigh_iact * sizeof(float));

  /* and fill them up */
  for (int i = 0; i < p->nneigh_iact; i++){
    p->neigh_iact[i] = neighcpy[i];
    p->r[i] = r[i];
  }


  free(neighcpy);

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
