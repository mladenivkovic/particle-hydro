#!/usr/bin/env python3

#---------------------------------------------------
# Create 2D uniform ICs. Assign random values to
# physical quantities.
#---------------------------------------------------


import numpy as np
from hydro_io import write_ic
import hydro_io


nx = 50

npart = nx

m = np.random.rand(npart)
u = np.random.rand(npart)
p = np.random.rand(npart)

dxhalf = 0.5/nx
x = np.linspace(dxhalf, 1-dxhalf, nx)

write_ic("random-uniform-1D-{0:d}.dat".format(npart), 1, x, m, u, p)
