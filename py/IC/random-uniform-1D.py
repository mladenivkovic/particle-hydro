#!/usr/bin/env python3

# ---------------------------------------------------
# Create 2D uniform ICs. Assign random values to
# physical quantities.
# ---------------------------------------------------


import numpy as np
from particle_hydro_io import write_ic
from particle_hydro_IC import IC_uniform_coordinates


nx = 50

npart = nx

m = np.random.rand(npart)
u = np.random.rand(npart)
p = np.random.rand(npart)
x = IC_uniform_coordinates(nx, ndim=1)

write_ic("random-uniform-1D-{0:d}.dat".format(npart), 1, x, m, u, p)
