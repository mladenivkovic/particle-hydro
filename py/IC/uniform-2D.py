#!/usr/bin/env python3

#---------------------------------------------------
# Create 2D uniform ICs. Everything is identical.
# instead of npart, specify nx: How many particles 
# in any direction you want to.
#---------------------------------------------------


import numpy as np
from hydro_io import write_ic
import hydro_io


nx = 10
m_all = 1.
ux_all = 0.
uy_all = 0.
p_all = 1.



npart = nx*nx

m = np.ones(npart, dtype=np.float) * m_all
u = np.zeros((npart, 2), dtype=np.float)
u[:, 0] = ux_all
u[:, 1] = uy_all
p = np.ones(npart, dtype=np.float) * p_all

dxhalf = 0.5/nx
xcoords = np.linspace(dxhalf, 1-dxhalf, nx)
ycoords = np.linspace(dxhalf, 1-dxhalf, nx)
x,y = np.meshgrid(xcoords, ycoords)
x = np.stack((x.ravel(), y.ravel()), axis=1)


write_ic("uniform-2D-{0:d}.dat".format(npart), 2, x, m, u, p)
