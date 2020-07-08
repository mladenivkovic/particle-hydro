#!/usr/bin/env python3

#---------------------------------------------------
# Create 2D uniform ICs. Assign random values to
# physical quantities.
# instead of npart, specify nx: How many particles 
# in any direction you want to.
#---------------------------------------------------


import numpy as np
from particle_hydro_io import write_ic
from particle_hydro_IC import IC_uniform_coordinates


nx = 20

npart = nx*nx

m = np.random.rand(npart)
u = np.random.rand(npart, 2)
p = np.random.rand(npart)
x = IC_uniform_coordinates(nx, ndim=2)

write_ic("random-uniform-2D-{0:d}.dat".format(npart), 2, x, m, u, p)
