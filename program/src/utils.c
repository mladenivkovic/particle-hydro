/* Misc utils that don't fit anywhere else*/

/* Written by Mladen Ivkovic, JAN 2020
 * mladen.ivkovic@hotmail.com           */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "utils.h"

extern params pars;

void print_header() {
  /*----------------------*/
  /* Print a nice header  */
  /*----------------------*/
  printf("====================================================================="
         "=================\n");
  printf("______  ___  ______ _____ _____ _____  _      _____    _   ___   "
         "_____________ _____ \n");
  printf("| ___ \\/ _ \\ | ___ \\_   _|_   _/  __ \\| |    |  ___|  | | | \\ "
         "\\ / /  _  \\ ___ \\  _  |\n");
  printf("| |_/ / /_\\ \\| |_/ / | |   | | | /  \\/| |    | |__    | |_| |\\ V "
         "/| | | | |_/ / | | |\n");
  printf("|  __/|  _  ||    /  | |   | | | |    | |    |  __|   |  _  | \\ / | "
         "| | |    /| | | |\n");
  printf("| |   | | | || |\\ \\  | |  _| |_| \\__/\\| |____| |___   | | | | | "
         "| | |/ /| |\\ \\\\ \\_/ /\n");
  printf("\\_|   \\_| |_/\\_| \\_| \\_/  \\___/ \\____/\\_____/\\____/   \\_| "
         "|_/ \\_/ |___/ \\_| \\_|\\___/ \n");
  printf("\n");
  printf("====================================================================="
         "=================\n");
  printf("\n");
}

void print_compile_defines() {
  /* ----------------------------------- */
  /* print compile time definitions      */
  /* ----------------------------------- */

  char solver[80];
  char kernel[80];

  utils_get_macro_strings(solver, kernel);

  log_message("----------------------------------------------------------------"
              "-----\n");
  log_message("\n");
  log_message("Compile time parameters are:\n");
  log_message("\n");

  log_message("Compile time:                " STR(COMPDATE) "\n");
  log_message("Dimensions:                  " STR(NDIM) "\n");
  log_message("Hydro solver:                %s\n", solver);
  log_message("Kernel:                      %s\n", kernel);
}

void utils_get_macro_strings(char *solver, char *kernel) {
  /* --------------------------------------------
   * Get string names for the solver in use.
   * -------------------------------------------- */

#if SOLVER == SPH_DS
  strcpy(solver, "SPH_DS");
#elif SOLVER == MESHLESS
  strcpy(solver, "MESHLESS");
#elif SOLVER == MESHLESS_IVANOVA
  strcpy(solver, "MESHLESS_IVANOVA");
#endif

#if KERNEL == CUBIC_SPLINE
  strcpy(kernel, "cubic spline");
#elif KERNEL == QUARTIC_SPLINE
  strcpy(kernel, "quartic spline");
#elif KERNEL == QUINTIC_SPLINE
  strcpy(kernel, "quintic spline");
#elif KERNEL == WENDLAND_C2
  strcpy(kernel, "wendland C2");
#elif KERNEL == WENDLAND_C4
  strcpy(kernel, "wendland C4");
#elif KERNEL == WENDLAND_C6
  strcpy(kernel, "wendland C6");
#endif
}

void log_message(const char *format, ...) {
  /*------------------------------------------------------
   * just prepends [LOG] to printing. Use like printf()
   *------------------------------------------------------*/

  if (pars.verbose < 1)
    return;

  printf("%-12s", "[LOG] ");

  va_list arg; /* from stdarg.h; va_list type defined in stdarg.h */
  int done;

  va_start(arg, format); /* initialises arg variable, starting with "format" */
                         /* variable given as an argument to print() */

  done = vfprintf(stdout, format, arg); /* call the formatting and printing */
                                        /* function that printf also uses */

  va_end(arg); /* do whatever cleanup is necessary */

  if (done < 0)
    throw_error("My own log_message() function exited with error code %d",
                done);
}

void debugmessage(const char *format, ...) {
  /*---------------------------------------------------------------
   * if verbose is 3 or higher, write whatever you want to write
   * to screen. Use it like you use printf(), except this function
   * will add a newline by itself :)
   *---------------------------------------------------------------*/

  if (pars.verbose < 3)
    return;

  printf("%-12s", "[DEBUGGING] ");

  va_list arg; /* from stdarg.h; va_list type defined in stdarg.h */
  int done;

  va_start(arg, format); /* initialises arg variable, starting with "format" */
                         /* variable given as an argument to print() */

  done = vfprintf(stdout, format, arg); /* call the formatting and printing */
                                        /* function that printf also uses */

  va_end(arg); /* do whatever cleanup is necessary */

  printf("\n");

  if (done < 0)
    throw_error("My own debugmessage() function exited with error code %d",
                done);
}

void log_extra(const char *format, ...) {
  /*-----------------------------------------------
   * if verbose is 2 or higher, write whatever you
   * want to write to screen. Use it like you use
   * printf(), except this function will add a
   * newline by itself :) .
   *-----------------------------------------------*/

  if (pars.verbose < 2)
    return;

  printf("%-12s", "[EXTRA] ");

  va_list arg; /* from stdarg.h; va_list type defined in stdarg.h */
  int done;

  va_start(arg, format); /* initialises arg variable, starting with "format" */
                         /* variable given as an argument to print() */

  done = vfprintf(stdout, format, arg); /* call the formatting and printing */
                                        /* function that printf also uses */

  va_end(arg); /* do whatever cleanup is necessary */

  printf("\n");

  if (done < 0)
    throw_error("My own log_extra() function exited with error code %d", done);
}

void throw_error(const char *format, ...) {
  /*-----------------------------------------------
   * Print a formatted error message to screen.
   * Use it like you use printf(), except this
   * function will add a newline by itself :) .
   * Then it will exit.
   *-----------------------------------------------*/

  printf("%-12s", "[ERROR]");

  va_list arg; /* from stdarg.h; va_list type defined in stdarg.h */
  int done;

  va_start(arg, format); /* initialises arg variable, starting with "format" */
                         /* variable given as an argument to print() */

  done = vfprintf(stdout, format, arg); /* call the formatting and printing */
                                        /* function that printf also uses */

  va_end(arg); /* do whatever cleanup is necessary */

  printf("\n"); /* always end with a newline! :) */

  if (done < 0)
    throw_error(
        "ERROR: your errormessage() function exited with error code %d\n",
        done);

  printf("GG yall, I'm out\n");
  exit(1);
}

void printbool(int boolean) {
  /*-------------------------------------*/
  /* prints "True" or "False"            */
  /*-------------------------------------*/

  if (boolean) {
    printf("true");
  } else {
    printf("false");
  }
}

float to_ndim_power(float x) {
  /* ----------------------------------
   * Return x^ndim
   * ---------------------------------- */
#if NDIM == 1
  return (x);
#elif NDIM == 2
  return (x * x);
#endif
}
