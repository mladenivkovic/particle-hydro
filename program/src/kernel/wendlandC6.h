/* Wendland C6 Header File*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#ifndef WENDLAND_C6_H
#define WENDLAND_C6_H

#if NDIM == 1

// 55 / 32
#define KERNEL_NORM 1.718750
#define KERNEL_Hoverh 2.207940


#elif NDIM == 2

// 78 / (7 pi)
#define KERNEL_NORM 3.546882
#define KERNEL_Hoverh 2.415230

#endif


#endif
