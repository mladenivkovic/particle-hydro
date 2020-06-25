/* top level file for macro definitions to be used throughout the code*/

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */



#ifndef DEFINES_H
#define DEFINES_H


/* -------------------------------------------------------------------
 * some behaviour options you are free to tinker with
 * ------------------------------------------------------------------- */

/* whether to use an adaptive, or a simple wave speed estimate for the HLLC
 * Riemann solver */
#define HLLC_USE_ADAPTIVE_SPEED_ESTIMATE


/* Physical constants */

#define GAMMA  (5./3.)
// #define GAMMA 1.4

/* boxsize */
#define BOXLEN 1.





/* ---------------------------------------------------------------------------------- *
 * you shouldn't be modifying stuff below
 * this line unless you know what you're doing
 * ---------------------------------------------------------------------------------- */

/* File related stuff */

#define MAX_FNAME_SIZE 200    /* limit for file name size */
#define MAX_LINE_SIZE 200     /* limit for line length in formatted file which is read in*/


/* Macro functions */

#define STR(x) STR_(x)
#define STR_(x) #x

/* iteration tolerance exact Riemann solver*/
#define EPSILON_ITER 1e-6

/* smoothing length iteration tolerance */
#define EPSILON_H 1e-3
/* max number of iterations to determine smoothing length */
#define ITER_MAX_H 1000


/* minimal timestep size */
#define DT_MIN 1e-10

/* "nonzero zeros" for vacuum */
#define SMALLRHO 0.
#define SMALLU 0.
#define SMALLP 0.


/* assumption for number of particles per cell */
#define PARTS_ARRAY_SIZE 100


/* for grid building: require that every cell has at lest
 * CELL_MIN_PARTS_IN_NEIGHBOURHOOD_FACT * nngb particles 
 * within all the neighbours combined */
#define CELL_MIN_PARTS_IN_NEIGHBOURHOOD_FACT 2.





/* ---------------------------------------------------------------------------------- *
 * Nobody should be changing things below this line
 * ----------------------------------------------------------------------------------*/


/* Compute gamma related constants */

static const float GM1 = GAMMA - 1.;
static const float GP1 = GAMMA + 1.;
static const float GP1OGM1 = (GAMMA + 1.) / (GAMMA - 1.);
static const float GM1OGP1 = (GAMMA - 1.) / (GAMMA + 1.);
static const float ONEOVERGAMMA = 1. / GAMMA;
static const float GM1HALF = 0.5 * (GAMMA - 1.);
static const float BETA = 0.5 * (GAMMA - 1.) / GAMMA;


/* PI */
#define PI 3.14159265 /* I know there are better ways of doing this. Sue me. */

/* define solvers as integers */
#define SPH_DS 11
#define MESHLESS 21
#define MESHLESS_IVANOVA 22

/* define riemann solvers as integers */
#define NONE 0
#define EXACT 1
#define HLLC 2
#define TRRS 3
#define TSRS 4

/* define limiters as integers */
#define MINMOD 1
#define SUPERBEE 2
#define VANLEER 3
#define MC 4

/* define kernels as integers */
#define CUBIC_SPLINE 11
#define QUARTIC_SPLINE 12
#define QUINTIC_SPLINE 13
#define WENDLAND_C4 21
#define WENDLAND_C5 22
#define WENDLAND_C6 23

/* define sources as integers */
#define SRC_CONST 1
#define SRC_RADIAL 2


#endif
