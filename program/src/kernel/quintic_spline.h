/* Quintic Spline Kernel Header File*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef QUINTIC_SPLINE_H
#define QUINTIC_SPLINE_H

#if NDIM == 1

// 3^5 / 40
#define KERNEL_NORM 6.075000
#define KERNEL_Hoverh 2.121321


#elif NDIM == 2

// 3^7 * 7 / (478 pi)
#define KERNEL_NORM 10.194573
#define KERNEL_Hoverh 2.158131


#endif


#endif
