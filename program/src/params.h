/* All around parameters used in the simulation. */

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */


#ifndef PARAMS_H
#define PARAMS_H

#include "defines.h"

/* GLOBAL PARAMETERS */
typedef struct {

  /* Talking related parameters */
  int verbose;                          /* how talkative I am */
  int nstep_log;                        /* on how many steps to write log message of time step */

  /* simulation related parameters */
  int npart;                            /* number of particls */
  int nsteps;                           /* how many steps to take */
  float tmax;                           /* at what time to end */

  float ccfl;                           /* CFL coefficient */
  float force_dt;                       /* force a time step size (except if you need to write an output) */
  int boundary;                         /* boundary condition for all walls.*/
  float nngb;                           /* number of neighbours to use */

  int nx;                               /* number of grid points */
  float dx;                             /* cell size */
  int ncelltot;                         /* total number of cells in grid. nx in 1D, nx^2 in 2D. Mainly used to avoid dimension checks */
  
  /* output related parameters */
  int foutput;                          /* after how many steps to write output */
  float dt_out;                         /* time interval between outputs */
  char outputfilename[MAX_FNAME_SIZE];  /* Output file name basename */

  char toutfilename[MAX_FNAME_SIZE];    /* file name containing output times */
  int use_toutfile;                     /* whether we're using the t_out_file */
  int noutput_tot;                      /* how many outputs we will be writing. Only used if(use_toutfile) */
  int noutput;                          /* at which output we are. Only used if(use_toutfile) */
  float *outputtimes;                   /* array of output times given in the output file */


  /* read in file parameters */
  char datafilename[MAX_FNAME_SIZE];    /* IC data filename */

  char paramfilename[MAX_FNAME_SIZE];   /* parameter filename */



  /* Sources related parameters */
  float src_const_acc_x;                /* constant acceleration in x direction for constant source terms */
  float src_const_acc_y;                /* constant acceleration in y direction for constant source terms */
  float src_const_acc_r;                /* constant acceleration in radial direction for radial source terms */
  int constant_acceleration;            /* whether the sources will be constant */
  int constant_acceleration_computed;   /* whether the constant acceleration has been computed */
  int sources_are_read;                 /* whether sources have been read in */

} params;






void params_init_defaults();
void params_init_derived();
void params_print_log();
void params_check();

#endif
