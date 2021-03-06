/* =======================================================
 * mesh based Conservation Law solver !
 * run with ./hydro parameterfile.txt ICfile.dat
 * See the README.md in parent directory on how the
 * parameter and IC file can look like.
 *
 * Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com
 * ======================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* measure time */

#include "cell.h"
#include "defines.h"
#include "gas.h"
#include "io.h"
#include "kernel.h"
#include "params.h"
#include "particles.h"
#include "solver.h"
#include "utils.h"

/* ------------------ */
/* Initialize globals */
/* ------------------ */

params pars;     /* global parameters */
part *particles; /* particle array */
cell *grid;      /* particle grid */

/* ====================================== */
int main(int argc, char *argv[]) {
  /* ====================================== */

  /* timing stuff */
  /* clock_t step_start, step_end; */
  clock_t all_start, all_end;

  all_start = clock();

  /* Unnecessary things first :) */
  print_header();

  /* initialize parameters struct */
  params_init_defaults();

  /* read cmdline args and parameter file */
  io_read_cmdlineargs(argc, argv);
  io_read_paramfile();

  /* read in the IC file */
  /* In this function, the particle array is allocated */
  io_read_ic();

  /* read in output times if necessary */
  if (pars.use_toutfile)
    io_read_toutfile();

  params_check();        /* check whether we can work with this setup. */
  params_init_derived(); /* process the parameters you got. */

  /* print / announce stuff for logging */
  print_compile_defines();
  params_print_log();

  cell_build_grid();
  part_get_smoothing_lengths();
  part_write_smoothing_lengths(0);
  free_part_arrays();  /* TODO: don't forget this! */
  cell_destroy_grid(); /* TODO: don't forget this! */

  /* temporary: to check kernels */
  /* printf("--------------------------------------------------------------\n");
   */
  /* for (int i = 0; i < 101; i++){ */
  /*   float r = 0.02 * (float) i; */
  /*   float h = 1. / KERNEL_Hoverh; */
  /*   printf("%.6f, %.6f, %.6f\n", r, kernel_W(r, h), kernel_dWdr(r, h)); */
  /* } */
  /* printf("--------------------------------------------------------------\n");
   */

  /* Initialize counters and time */
  int step = 0;     /* step counter */
  int outcount = 0; /* number of the output that we're writing */
  float t = 0;      /* time */
  /* float dt = 0;         [> time step size <] */

  int write_output = 0; /* whether the time step was reduced because we need to
                           write an output */

  log_extra("Writing initial output");
  io_write_output(&outcount, step, t);

  log_message("\n");
  log_message("%14s %14s %14s  %14s\n", "step", "time", "dt", "time step took");

  /* --------------------
   *   Main loop
   * -------------------- */
  /* while(0) { [> TODO: set to false temporary <] */
  /*   if (pars.tmax > 0 && t >= pars.tmax) break; */
  /*   if (pars.nsteps>0 && step == pars.nsteps) break; */
  /*  */
  /*   step_start = clock(); [> timer <] */
  /*  */
  /*   [> initialize the grid <] */
  /*   cell_init_grid(); */
  /*  */
  /*   [> where the actual magic happens <] */
  /*   solver_step(&t, &dt, step,  &write_output); */
  /*  */
  /*   step_end = clock(); [> timer <] */
  /*  */
  /*   [> and update time and step<] */
  /*   t += dt; */
  /*   step += 1; */
  /*  */
  /*   [> write output if you have to <] */
  /*   if (write_output){ */
  /*     io_write_output(&outcount, step, t); */
  /*   } */
  /*  */
  /*   [> announce <] */
  /*   if (pars.nstep_log == 0 || step % pars.nstep_log == 0) { */
  /*     log_message("%14d %14.6e %14.6e %14.3es\n", */
  /*                 step, t, dt, (float)(step_end - step_start) /
   * CLOCKS_PER_SEC); */
  /*   } */
  /* } */

  /* if you haven't written the output in the final step, do it now */
  if (!write_output) {
    io_write_output(&outcount, step, t);
  }

  all_end = clock();

  /* don't use log_message, I want final stats even for verbose = 0 */
  printf("\n");
  printf("  Finished clean. Yay!\n");
  printf("  Final stats:\n");
  printf("\n");
  printf("    Total runtime was       %12.6fs\n",
         (float)(all_end - all_start) / CLOCKS_PER_SEC);
  printf("    final number of steps = %12d\n", step);

  return (0);
}
