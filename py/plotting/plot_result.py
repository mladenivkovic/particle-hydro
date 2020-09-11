#!/usr/bin/env python3


# -------------------------------------
# Create a plot of an output file.
#
#  usage: plot_result.py [-h] [-i] [-p] [-s] filenames [filenames ...]
#
#  positional arguments:
#    filenames      file(s) to plot
#
#  optional arguments:
#    -h, --help     show this help message and exit
#    -i, --image    make an image [default]. For 1D data, this creates a
#                   scatterplot.
#    -p, --profile  plot profiles. For 1D data, this has no effect.
#  -s, --scatter  make a scatterplot. For 1D data, this has no effect.
# -------------------------------------


# first things first: check whether you can import the hydro python modules
from check_module_is_in_pythonpath import try_to_import

try_to_import()


from particle_hydro_utils import parse_arguments, label_to_kwargs
from particle_hydro_io import read_output
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

    ndim, x, m, rho, u, p, h, t, step = read_output(fname)

    if ndim == 1:
        fig = plot_1D(x, rho, u, p)
        save_plot(
            fig, fname,
        )
    elif ndim == 2:
        if case == "image":
            fig = plot_2D(x, rho, u, p, t=t)
        elif case == "profile":
            fig = plot_2D_profile(x, rho, u, p, t=t)
        elif case == "scatter":
            fig = scatter_2D(x, rho, u, p, t=t)
        save_plot(fig, fname, plottype=case)
