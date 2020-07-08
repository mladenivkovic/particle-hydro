#!/usr/bin/env python3

#--------------------------------------------
# IC generation related stuff
#--------------------------------------------


import numpy as np



def IC_uniform_coordinates(nx, ndim = 2, periodic = True):
    """
    Get the coordinates for a uniform particle distribution.
    nx:         number of particles in each dimension
    ndim:       number of dimensions
    periodic:   whether we have periodic boundary conditions or not

    returns:
        x: np.array((nx**ndim, ndim-1), dtype=float) of coordinates
    """

    dxhalf = 0.5/nx

    if ndim == 1:
        x = np.linspace(dxhalf, 1-dxhalf, nx)

    elif ndim == 2:
        xcoords = np.linspace(dxhalf, 1-dxhalf, nx)
        ycoords = np.linspace(dxhalf, 1-dxhalf, nx)
        x,y = np.meshgrid(xcoords, ycoords)
        x = np.stack((x.ravel(), y.ravel()), axis=1)

    return x





def IC_perturbed_coordinates(nx, ndim = 2, periodic = True):
    """
    Get the coordinates for a perturbed uniform particle distribution.
    The perturbation won't exceed the half interparticle distance
    along an axis.

    nx:         number of particles in each dimension
    ndim:       number of dimensions
    periodic:   whether we have periodic boundary conditions or not

    returns:
        x: np.array((nx**ndim, ndim-1), dtype=float) of coordinates
    """

    x = IC_uniform_coordinates(nx, ndim=ndim, periodic=periodic)
    dxhalf = 0.5/nx
    if ndim == 1:
        for i in range(nx):
            sign = 1 if np.random.random() < 0.5 else -1
            x[i] += sign * np.random.random() * dxhalf
    elif ndim == 2:
        for i in range(nx):
            sign = 1 if np.random.random() < 0.5 else -1
            x[i][0] += sign * np.random.random() * dxhalf
            sign = 1 if np.random.random() < 0.5 else -1
            x[i][1] += sign * np.random.random() * dxhalf

    return x



