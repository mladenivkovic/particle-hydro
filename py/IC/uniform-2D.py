#!/usr/bin/env python3

# ---------------------------------------------------
# Create 2D uniform ICs. Everything is identical.
# instead of npart, specify nx: How many particles
# in any direction you want to.
# ---------------------------------------------------


import numpy as np
from particle_hydro_io import write_ic
from particle_hydro_IC import IC_uniform_coordinates
from particle_hydro_grid import compute_smoothing_lengths


nx = 50
m_all = 1.0
ux_all = 0.0
uy_all = 0.0
p_all = 1.0


npart = nx * nx

m = np.ones(npart, dtype=np.float) * m_all
m[nx // 2 * nx :] *= 2
u = np.zeros((npart, 2), dtype=np.float)
u[:, 0] = ux_all
u[:, 1] = uy_all
p = np.ones(npart, dtype=np.float) * p_all
x = IC_uniform_coordinates(nx, ndim=2)

write_ic("uniform-2D-{0:d}.dat".format(npart), 2, x, m, u, p)
