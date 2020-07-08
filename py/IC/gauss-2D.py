#!/usr/bin/env python3

#------------------------------------------
# Create SPH initial conditions with a 
# gauss profile in 2D
# instead of specifying how many particles
# you want in total, specify nx, the number
# of particles in each dimension
#------------------------------------------


import numpy as np

from particle_hydro_IC import IC_perturbed_coordinates
from particle_hydro_grid import compute_smoothing_lengths


nx = 20
ndim = 2

x = IC_perturbed_coordinates(nx, ndim = ndim, periodic = False)
m = np.ones(x.shape[0])
h = compute_smoothing_lengths(x, m, 1.2345, ndim = 2)

print("=====================================")
print("=====================================")
print("=====================================")


nx = 100
ndim = 1

x = IC_perturbed_coordinates(nx, ndim = ndim, periodic = False)
m = np.ones(x.shape[0])
h = compute_smoothing_lengths(x, m, 1.2345, ndim = ndim, periodic = True)
