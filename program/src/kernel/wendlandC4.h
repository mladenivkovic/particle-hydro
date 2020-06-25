/* Wendland C4 Kernel Header File*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef WENDLAND_C4_H
#define WENDLAND_C4_H

#if NDIM == 1

// 3/2
#define KERNEL_NORM 1.500000
#define KERNEL_Hoverh 1.936492


#elif NDIM == 2

// 9/pi
#define KERNEL_NORM 2.864789
#define KERNEL_Hoverh 2.171239


#endif


#endif
