#!/usr/bin/env python3

#---------------------------------------------------
# Create 2D uniform ICs. Assign random values to
# physical quantities.
# instead of npart, specify nx: How many particles 
# in any direction you want to.
#---------------------------------------------------


import numpy as np
from particle_hydro_io import write_ic


nx = 20

npart = nx*nx

m = np.random.rand(npart)
u = np.random.rand(npart, 2)
p = np.random.rand(npart)

dxhalf = 0.5/nx
xcoords = np.linspace(dxhalf, 1-dxhalf, nx)
ycoords = np.linspace(dxhalf, 1-dxhalf, nx)
x,y = np.meshgrid(xcoords, ycoords)
x = np.stack((x.ravel(), y.ravel()), axis=1)


write_ic("random-uniform-2D-{0:d}.dat".format(npart), 2, x, m, u, p)
