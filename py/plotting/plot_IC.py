#!/usr/bin/env python3


# -------------------------------------
# Create a plot of an IC file.
# Usage:
#   plot_IC.py file.dat
# -------------------------------------


# first things first: check whether you can import the hydro python modules
from check_module_is_in_pythonpath import try_to_import

try_to_import()


from particle_hydro_utils import parse_arguments, label_to_kwargs
from particle_hydro_io import read_ic
from particle_hydro_plotting import (
    plot_1D,
    plot_2D,
    plot_2D_profile,
    scatter_2D,
    save_plot,
)


if __name__ == "__main__":

    case, inputfiles = parse_arguments()
    if len(inputfiles) > 1:
        print("This script only works with 1 input file, you gave me", len(inputfiles))
        quit()
    fname = inputfiles[0]

    ndim, x, m, u, p = read_ic(fname)
    print("WARNING: Not plotting density, but mass")

    if ndim == 1:
        fig = plot_1D(x, m, u, p,)
        save_plot(
            fig, fname,
        )
    elif ndim == 2:
        if case == "image":
            fig = plot_2D(x, m, u, p,)
        elif case == "profile":
            fig = plot_2D_profile(x, m, u, p,)
        elif case == "scatter":
            fig = scatter_2D(x, m, u, p,)
        save_plot(fig, fname, plottype=case)
