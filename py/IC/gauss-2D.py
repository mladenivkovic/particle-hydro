#!/usr/bin/env python3

#------------------------------------------
# Create SPH initial conditions with a 
# gauss profile in 2D
# instead of specifying how many particles
# you want in total, specify nx, the number
# of particles in each dimension
#------------------------------------------


import numpy as np
from scipy import stats

from particle_hydro_IC import IC_perturbed_coordinates
from particle_hydro_grid import compute_smoothing_lengths
from particle_hydro_IC import generate_IC_for_given_density
from particle_hydro_IC import IC_generation_set_params
from particle_hydro_IC import IC_sample_coordinates

def rho(x):
    """
    Analytical function to be used
    """
    
    if x.ndim == 1:
        return 1 - (x - 0.5)**2
    elif x.ndim == 2:
        return 1 - (x[:,0] - 0.5)**2 - (x[:,1] - 0.5)**2
    #  return np.ones(x.shape[0], dtype=np.float)



IC_generation_set_params(
            iter_max                    = 1000,
            convergence_threshold       = 2e-3,
            tolerance_part              = 1e-2,
            displacement_threshold      = 5e-3,
            redistribute_at_iteration   = 10,
            delta_reduction_factor      = 0.99,
            delta_min                   = 1e-4,
            redistribute_fraction       = 0.01,
            no_redistribution_after     = 120,
            plot_at_redistribution      = True
        )

ndim = 2
nx = 100
#
#
#  x = IC_perturbed_coordinates(nx, ndim = ndim, periodic = True)
x = IC_sample_coordinates(nx, rho, ndim=ndim)
#  #  m = np.ones(x.shape[0])
x, m, rho_comp, h = generate_IC_for_given_density(rho, nx, ndim, 1.2348, x=x)






#  print("=====================================")
#  print("=====================================")
#  print("=====================================")


nx = 1000
ndim = 1

#  x = IC_perturbed_coordinates(nx, ndim = ndim, periodic = False)
#  m = np.ones(x.shape[0])
#  h, rho, grid, neighbours = compute_smoothing_lengths(x, m, 1.2345, ndim = ndim)
#  print(neighbours)

#  generate_IC_for_given_density(rho, nx, ndim)
#  x, m, rho_comp, h = generate_IC_for_given_density(rho, nx, ndim, 1.2348, periodic=False)
