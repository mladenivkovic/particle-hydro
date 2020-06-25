/* sotring routines*/

/* Written by Mladen Ivkovic, JUN 2020
 * mladen.ivkovic@hotmail.com           */

#include "sort.h"


void quicksort_float_int_follower(float* arr, int* follower, int len){
  /* -----------------------------------------------------------
   * Top level quicksort function. 
   * Calls quicksort_float_int_recursive().
   * arr: array to be sorted.
   * follower: array of same length as arr. Will be sorted the
   *           same way arr is being sorted, but following the
   *           order of arr.
   * len: length of arrays
   * ----------------------------------------------------------- */

  quicksort_float_int_follower_recursive(arr, follower, 0, len-1);
}







void quicksort_float_int_follower_recursive(float* arr, int* follower, int lo, int hi){
  /* -----------------------------------------------------------
   * Recursive quicksort function.
   * arr: array to be sorted.
   * follower: array of same length as arr. Will be sorted the
   *           same way arr is being sorted, but following the
   *           order of arr.
   * len: length of arrays
   * lo:  lower index
   * hi:  higher index
   * ----------------------------------------------------------- */

  if (lo < hi){
    /* pick a pivot */
    float pivot = arr[(lo + hi)/2];

    /* partition array */
    int i = lo;
    int j = hi;

    /* loop until i >= j */
    while (i <= j){
      
      /* loop until you find index i where value > pivot from the left*/
      while (arr[i] < pivot){
        i += 1;
      }

      /* loop until you find index j where value < pivot from the right */
      while (arr[j] > pivot){
        j -= 1;
      }

      /* swap every instance you found */
      if (i <= j) {

        /* first the actual array to be sorted */
        float tempf = arr[i];
        arr[i] = arr[j];
        arr[j] = tempf;

        /* now the follower */
        int tempi = follower[i];
        follower[i] = follower[j];
        follower[j] = tempi;

        i += 1;
        j -= 1;
      }
    }

    /* call recursively */
    quicksort_float_int_follower_recursive(arr, follower, lo, j);
    quicksort_float_int_follower_recursive(arr, follower, i, hi);

  }
}
