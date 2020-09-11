/* Quartic Spline Kernel Header File*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef QUARTIC_SPLINE_H
#define QUARTIC_SPLINE_H

#if NDIM == 1

// 5^5 / 786
#define KERNEL_NORM 4.069010
#define KERNEL_Hoverh 1.936492

#elif NDIM == 2

// 5^6 * 3 / (2398 pi)
#define KERNEL_NORM 6.222175
#define KERNEL_Hoverh 1.977173

#endif

#endif
