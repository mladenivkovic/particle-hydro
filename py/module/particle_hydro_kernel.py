#!/usr/bin/env python3


#------------------------------------
# Kernel related stuffs
#------------------------------------



def W_cubic_spline(r, H, ndim = 2):
    """
    Cubic spline kernel.
    """

    q = r / H
    W = 0.

    if q <= 1.:
        W += (1. - q)**3
    if q <= 0.5:
        W -= 4*(0.5 - q)**3

    if ndim == 1:
        W /= H
        W *= kernel_norm_1D['cubic spline']
    elif ndim == 2:
        W /= H**2
        W *= kernel_norm_2D['cubic spline']

    return W




def dWdr_cubic_spline(r, H, ndim = 2):
    """
    Cubic spline kernel derivative
    """

    q = r / H
    W = 0.

    if q <= 1.:
        W -= 3 * (1. - q)**2
    if q <= 0.5:
        W += 12*(0.5 - q)**2


    if ndim == 1:
        W /= H**2
        W *= kernel_norm_1D['cubic spline']
    elif ndim == 2:
        W /= H**3
        W *= kernel_norm_2D['cubic spline']

    return W



kernel_gamma_1D = {}
kernel_gamma_1D['cubic spline'] = 1.732051

kernel_norm_1D = {}
kernel_norm_1D['cubic spline'] = 2.666667

kernel_func_1D = {}
kernel_func_1D['cubic spline'] = W_cubic_spline

kernel_derivative_1D = {}
kernel_derivative_1D['cubic spline'] = dWdr_cubic_spline



kernel_gamma_2D = {}
kernel_gamma_2D['cubic spline'] = 1.778002

kernel_norm_2D = {}
kernel_norm_2D['cubic spline'] = 3.637827

kernel_func_2D = {}
kernel_func_2D['cubic spline'] = W_cubic_spline

kernel_derivative_2D = {}
kernel_derivative_2D['cubic spline'] = dWdr_cubic_spline





def get_kernel_data(kernel, ndim):
    """
    Picks the correct kernel functions and constants for you.
    Returns:
    W(r, H):        kernel function
    dWdr(r, H):     kernel derivative function
    kernel_gamma:   H/h for given kernel and dimension
    
    """
    if ndim == 1:
        W = lambda r, H : kernel_func_1D[kernel](r, H, ndim=1)
        dWdr = lambda r, H : kernel_derivative_1D[kernel](r, H, ndim=1)
        kernel_gamma = kernel_gamma_1D[kernel]
    elif ndim == 2:
        W = lambda r, H : kernel_func_2D[kernel](r, H, ndim=2)
        dWdr = lambda r, H : kernel_derivative_2D[kernel](r, H, ndim=2)
        kernel_gamma = kernel_gamma_2D[kernel]
    return W, dWdr, kernel_gamma

