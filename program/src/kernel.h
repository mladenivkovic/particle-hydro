/* Kernel top level header */

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef KERNEL_H
#define KERNEL_H

#include "defines.h"

#if KERNEL == CUBIC_SPLINE
#include "kernel/cubic_spline.h"
#elif KERNEL == QUARTIC_SPLINE
#include "kernel/quartic_spline.h"
#elif KERNEL == QUINTIC_SPLINE
#include "kernel/quintic_spline.h"
#elif KERNEL == WENDLAND_C2
#include "kernel/wendlandC2.h"
#elif KERNEL == WENDLAND_C4
#include "kernel/wendlandC4.h"
#elif KERNEL == WENDLAND_C6
#include "kernel/wendlandC6.h"
#endif



float kernel_W(float r, float h);
float kernel_dWdr(float r, float h);


float kernel_Hfromh(float h);
float kernel_hfromH(float H);




#endif
