/* Cubic Spline Kernel Header File*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef CUBIC_SPLINE_H
#define CUBIC_SPLINE_H

#if NDIM == 1

// 8/3
#define KERNEL_NORM 2.666667
#define KERNEL_Hoverh 1.732051

#elif NDIM == 2

// 80 / (7 pi)
#define KERNEL_NORM 3.637827
#define KERNEL_Hoverh 1.778002

#endif

#endif
