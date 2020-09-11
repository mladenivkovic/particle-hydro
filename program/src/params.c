/* global runtime parameter related stuff. */

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "params.h"
#include "utils.h"

extern params pars;

void params_init_defaults() {
  /*------------------------------------------*/
  /* Initialize parameters to default values  */
  /*------------------------------------------*/

  /* Talking related parameters */
  pars.verbose = 0;
  pars.nstep_log = 0;

  /* simulation related parameters */
  pars.npart = 100;
  pars.nsteps = 0;
  pars.tmax = 0;

  pars.ccfl = 0.9;
  pars.force_dt = 0;
  pars.boundary = 0;
  pars.nngb = 0.0;
  pars.eta = 0.0;

  pars.nx = pars.npart;
  pars.dx = BOXLEN / pars.npart;
  pars.ncelltot = pars.nx;

  /* output related parameters */
  pars.foutput = 0;
  pars.dt_out = 0;
  strcpy(pars.outputfilename, "");

  strcpy(pars.toutfilename, "");
  pars.use_toutfile = 0;
  pars.noutput_tot = 0;
  pars.noutput = 0;
  pars.outputtimes = NULL;

  /* read in file related parameters */
  strcpy(pars.datafilename, "");
  strcpy(pars.paramfilename, "");

  /* Sources related parameters */
  pars.src_const_acc_x = 0.;
  pars.src_const_acc_y = 0.;
  pars.src_const_acc_r = 0.;
  pars.constant_acceleration = 0;
  pars.constant_acceleration_computed = 0;
  pars.sources_are_read = 0;
}

void params_init_derived() {
  /* ------------------------------------------------- */
  /* Set up parameters that need some processing first */
  /* ------------------------------------------------- */

  /* Generate output filename based on ic filename */
  /*-----------------------------------------------*/
  /* do this only if no basename is given */
  if (strlen(pars.outputfilename) == 0) {

    int dot = 0;
    /* extract filename without suffix */
    for (int i = strlen(pars.datafilename); i > 0; i--) {
      if (pars.datafilename[i] == '.') {
        dot = i;
        break;
      }
    }

    int slash = 0;
    /* remove possible directories paths from filename*/
    for (int i = 0; i < (int)strlen(pars.datafilename); i++) {
      if (pars.datafilename[i] == '/') {
        slash = i;
      }
    }

    if (dot == 0)
      dot = strlen(pars.datafilename);
    if (slash > 0)
      slash += 1;

    char solver[80];
    char kernel[80];

    utils_get_macro_strings(solver, kernel);

    /* now copy the exact part that you want into filename string */
    strncpy(pars.outputfilename, pars.datafilename + slash, dot - slash);
    pars.outputfilename[dot - slash] = '\0'; /* safety measure */
    strcat(pars.outputfilename, "-");

    /* Add hydro solver */
    strcat(pars.outputfilename, solver);
    strcat(pars.outputfilename, "-");

    /* Add dimension */
    strcat(pars.outputfilename, STR(NDIM));
    strcat(pars.outputfilename, "D");
  }

  /* -----------------------------------------------------------------------------------
   * If the user set dt_out, pre-compute the output times just like we would
   * have if we used a outputtime file to specify the output times in order to
   * avoid misbehaviour with * floating point divisions
   * -----------------------------------------------------------------------------------
   */

  if (pars.dt_out > 0) {
    /* set the flag */
    pars.use_toutfile = 1;

    /* now get output times */
    int nout;

    if (pars.tmax > 0) {
      nout = floor(pars.tmax / pars.dt_out) + 1; /* add 1 for good measure */
    } else {
      /* assume maximally 9999 outputs */
      nout = 9999;
    }

    pars.outputtimes = malloc(nout * sizeof(float));
    pars.noutput_tot = nout;
    for (int i = 0; i < nout; i++) {
      pars.outputtimes[i] = (i + 1) * pars.dt_out;
    }
  }

  /* Compute nngb or eta, which ever is currently missing */
  /* ---------------------------------------------------- */

  if (pars.eta > 0.) {
#if NDIM == 1
    pars.nngb = 2 * KERNEL_Hoverh * pars.eta;
#elif NDIM == 2
    float temp = KERNEL_Hoverh * pars.eta;
    pars.nngb = temp * temp * PI;
#endif

  } else if (pars.nngb > 0.) {
#if NDIM == 1
    pars.eta = pars.nngb * 0.5 / KERNEL_Hoverh;
#elif NDIM == 2
    pars.eta = sqrtf(pars.nngb / PI) / KERNEL_Hoverh;
#endif
  }

  /* Make first estimate for cell size */
  /* --------------------------------- */

#if NDIM == 1
  pars.dx = BOXLEN / pars.npart * pars.nngb;
#elif NDIM == 2
  pars.dx = BOXLEN / sqrt(pars.npart / pars.nngb);
#endif

  pars.nx = (int)(BOXLEN / pars.dx) + 1;
  pars.dx = BOXLEN / (float)pars.nx;

  log_extra("Initial guess for grid parameters: nx=%d, dx=%.3f", pars.nx,
            pars.dx);

  /* Mark if we use constant sources */
  /* ------------------------------- */
#if (SOURCE == SRC_CONST) || (SOURCE == SRC_RADIAL)
  pars.constant_acceleration = 1;
#endif
}

void params_print_log() {
  /*------------------------------------------*/
  /* Print out current parameters             */
  /*------------------------------------------*/

  log_message("\n");
  log_message("Runtime parameters are:\n");
  log_message("\n");

  log_message("Verbose?                     ");
  if (pars.verbose > 0)
    printbool(pars.verbose);
  if (pars.verbose > 0)
    printf("\n");
  if (pars.nstep_log > 0) {
    log_message("Will write logs every %d steps\n", pars.nstep_log);
  }

  log_message("particles:                   %d\n", pars.npart);
  log_message("tmax:                        %g\n", pars.tmax);
  log_message("nsteps:                      %d\n", pars.nsteps);
  log_message("C_cfl:                       %g\n", pars.ccfl);
  log_message("Nngb:                        %.3f\n", pars.nngb);
  log_message("eta:                         %.3f\n", pars.eta);

  if (pars.force_dt > 0) {
    log_message("Forcing time step size to: %g\n", pars.force_dt);
  }

  log_message("boundary conditions:         ");
  if (pars.verbose > 0) {
    if (pars.boundary == 0) {
      printf("periodic\n");
    } else if (pars.boundary == 1) {
      printf("transmissive\n");
    }
  }

  log_message("IC file:                     %s\n", pars.datafilename);

  if (pars.use_toutfile) {
    if (pars.dt_out == 0.0) {
      log_message("Using output times file:     %s\n", pars.toutfilename);
    } else {
      log_message("dt_out:                      %g\n", pars.dt_out);
    }
  } else {
    log_message("foutput:                     %d\n", pars.foutput);
  }

  log_message("output file basename:        %s\n", pars.outputfilename);

#if SOURCE == SRC_CONST
  log_message("constant source in x:        %g\n", pars.src_const_acc_x);
  log_message("constant source in y:        %g\n", pars.src_const_acc_y);
#elif SOURCE == SRC_RADIAL
  log_message("constant source in r:        %g\n", pars.src_const_acc_r);
#endif

  log_message("----------------------------------------------------------------"
              "-----\n");
}

void params_check() {
  /* ----------------------------------------------- */
  /* Check whether we can work with these parameters */
  /* ----------------------------------------------- */

  log_extra("Checking whether we have valid parameters");

  if (pars.tmax == 0 && pars.nsteps == 0) {
    throw_error("In params_check: I have nsteps = 0 and tmax = 0. You need to "
                "tell me when to stop.");
  }

  if (pars.foutput < 0) {
    throw_error("foutput is negative. What do you expect me to do with that?");
  }

  if (pars.dt_out < 0) {
    throw_error("dt_out is negative. What do you expect me to do with that?");
  }

  if (pars.foutput > 0 && pars.dt_out > 0) {
    throw_error(
        "You specified dt_out and foutput > 0. You can't have both, pick one.");
  }

  if (pars.nngb == 0 && pars.eta == 0) {
    throw_error("Neigher nngb nor eta was specified in the parameter file. I "
                "need exactly one of these two to work.");
  }

  if (pars.nngb != 0 && pars.eta != 0) {
    throw_error("You gave me both nngb and eta in the parameter file. I need "
                "exactly one of these two to work.");
  }

  if (pars.nngb < 0)
    throw_error(
        "You gave me a negative nngb: %f. What am I supposed to do with that?",
        pars.nngb);
  if (pars.eta < 0)
    throw_error(
        "You gave me a negative eta: %f. What am I supposed to do with that?",
        pars.eta);

  if (pars.use_toutfile) {
    if (pars.dt_out != 0)
      throw_error("You gave me a output time file, but also a dt_out. Decide "
                  "which you want and retry.");
    if (pars.foutput != 0)
      throw_error("You gave me a output time file, but also a foutput. Decide "
                  "which you want and retry.");
  }

  if (pars.nx == 0) {
    throw_error("In params_check: I have nx = 0 cells for the sim.");
  }

  /* check source related stuff. */
#ifdef WITH_SOURCES
  if (!pars.sources_are_read) {
    /* Have we compiled with sources, but read in what the sources are? */
    throw_error("Code is compiled to work with sources, but I haven't read in "
                "any source related parameters.");
  }
#else
  /* have we compiled without sources, but read in sources? */
  if (pars.sources_are_read) {
    throw_error("Code is compiled to work without sources, but I read in "
                "source related parameters.");
  }
#endif
}
