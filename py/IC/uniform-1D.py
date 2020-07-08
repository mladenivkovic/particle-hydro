#!/usr/bin/env python3

#---------------------------------------------------
# Create 1D uniform ICs. Everything is identical.
#---------------------------------------------------


import numpy as np
from particle_hydro_io import write_ic
from particle_hydro_IC import IC_uniform_coordinates


nx = 20
m_all = 1.
u_all = 0.
p_all = 1.



npart = nx

m = np.ones(npart, dtype=np.float) * m_all
u = np.ones(npart, dtype=np.float) * u_all
p = np.ones(npart, dtype=np.float) * p_all
x = IC_uniform_coordinates(nx, ndim=1)

write_ic("uniform-1D-{0:d}.dat".format(npart), 1, x, m, u, p)
