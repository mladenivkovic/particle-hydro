/* Kernel top level file for common functions */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#include "kernel.h"

float kernel_Hfromh(float h){
  /* -----------------------------------
   * Get kernel support length from
   * smoothing length
   * ----------------------------------- */
  return(h*KERNEL_Hoverh);
}

float kernel_hfromH(float H){
  /* -----------------------------------
   * Get smoothing length from kernel
   * support length
   * ----------------------------------- */
  return(H/KERNEL_Hoverh);
}

