/* Wendland C2 Kernel Header File*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef WENDLAND_C2_H
#define WENDLAND_C2_H

#if NDIM == 1

// 5/4
#define KERNEL_NORM 1.250000
#define KERNEL_Hoverh 1.620185

#elif NDIM == 2

// 7 / pi
#define KERNEL_NORM 2.228169
#define KERNEL_Hoverh 1.897367

#endif

#endif
