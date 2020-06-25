/* Cubic Spline Kernel */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */


#include "kernel.h"
#include "utils.h"

#include <math.h>


float kernel_W(float r, float h){
  /* ---------------------------------------
   * Evaluate kernel value at given distance
   * r for given smoothing length h
   * --------------------------------------- */
 
  float H = h * KERNEL_Hoverh;
  float q = r / H;
  float w = 0.;

  float temp;

  if (q <= 1.) {
    temp = 1. - q;
    w += temp * temp * temp;
  }
  if (q <= 0.5) {
    temp = 0.5 - q;
    w -= 4 * temp * temp * temp;
  } 
  
#if NDIM == 1
  w /= H;
#elif NDIM == 2
  w /= H * H;
#endif
  w *= KERNEL_NORM;
  return(w);
}





float kernel_dWdr(float r, float h){
  /* ---------------------------------------
   * Evaluate first kernel derivative at 
   * given distance r for given smoothing 
   * length h
   * --------------------------------------- */
  
  float H = h * KERNEL_Hoverh;
  float q = r / H;
  float dwdr = 0.;

  float temp;

  if (q <= 1.) {
    temp = 1. - q;
    dwdr -= 3 * temp * temp;
  }
  if (q <= 0.5) {
    temp = 0.5 - q;
    dwdr += 12 * temp * temp;
  } 
  
  /* additional 1/h: dw/dr = dw/dq dq/dr = dw/dq 1/h */
#if NDIM == 1
  dwdr /= H * H;
#elif NDIM == 2
  dwdr /= H * H * H;
#endif
  dwdr *= KERNEL_NORM;
  return(dwdr);
}
