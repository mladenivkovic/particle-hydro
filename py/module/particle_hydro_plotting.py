#!/usr/bin/env python3


#---------------------------
# Plotting functions
#---------------------------


import matplotlib
from matplotlib import pyplot as plt
#  matplotlib.use("Agg")
from mpl_toolkits.axes_grid1 import make_axes_locatable, axes_size
from mpl_toolkits.mplot3d import Axes3D

import numpy as np
from scipy import stats


# Plot parameters
params = {
    'axes.labelsize': 12,
    'axes.titlesize': 14,
    'font.size': 12,
    'font.family': 'serif',
    'font.serif': 'DejaVu Sans',
    'legend.fontsize': 12,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'text.usetex': True,
    'figure.subplot.left'    : 0.05,
    'figure.subplot.right'   : 0.97,
    'figure.subplot.bottom'  : 0.12,
    'figure.subplot.top'     : 0.92,
    'figure.subplot.wspace'  : 0.25,
    'figure.subplot.hspace'  : 0.25,
    'figure.dpi' : 200,
    'lines.markersize' : 6,
    'lines.linewidth' : 2.
}

matplotlib.rcParams.update(params)

# set file format here if necessary. Allowed options: png, pdf
file_format = "png"
#  file_format = "pdf"





def plot_1D(x, rho, u, p, t = None, kwargs={}):
    """
    Create a plot from 2D data.
    x, rho, u, p:   np arrays of physical quantities
    t:           time of the simulation

    kwargs get passed to matplotlib.pyplot.scatter(), and need to be a dictionnary

    returns:
        fig:     pyplot.figure() object containing the plots
    """

    fig = plt.figure(figsize=(16, 5))

    nx = rho.shape[0]

    ax1 = fig.add_subplot(1, 3, 1)
    im1 = ax1.scatter(x, rho,
             **kwargs,
            )
    ax1.set_ylabel('density')

    ax2 = fig.add_subplot(1, 3, 2)
    im2 = ax2.scatter(x, u,
             **kwargs,
            )
    ax2.set_ylabel('velocity')

    ax3 = fig.add_subplot(1, 3, 3)
    im3 = ax3.scatter(x, p, 
             **kwargs,
            )
    ax3.set_ylabel('pressure')

    for ax in fig.axes:
        ax.set_xlabel("x")
        ax.set_xlim(0,1)
        #  ax.set_ylabel("y")
        ax.set_ylim(0,1)


    if t is not None:
        if isinstance(t, str):
            text = t
        elif isinstance(t, float):
            text = r"$t = ${0:.3f}".format(t)
        else:
            raise ValueError("Got weird data type for label (t). t=", t, "type(t)=", type(t))
        plt.figtext(0.05, 0.95, text)

    return fig











def plot_2D_density_only(rho, t=None, kwargs={}):
    """
    Create a plot from 2D data. Plots density only.

    rho:         np arrays of physical quantities
    fname:       filename of the data you are plotting. Will be used to generate image filename
    t:           time of simulation, optional. Will be put on the plot to label it. If it is a string, it will be used just as the label.

    kwargs get passed to matplotlib.pyplot.imshow(), and need to be a dictionnary

    returns:
        fig:     pyplot.figure() object containing the plots
    """

    fig = plt.figure(figsize=(6, 5))

    nx = rho.shape[0]
    rho = rho.transpose()

    ax1 = fig.add_subplot(1, 1, 1)
    im1 = ax1.imshow(rho,
            origin='lower', 
            extent=(0,1,0,1),
             **kwargs,
            )
    ax1.set_title('density')

    ax1.set_xlabel("x")
    ax1.set_xlim(0,1)
    ax1.set_ylabel("y")
    ax1.set_ylim(0,1)


    divider = make_axes_locatable(ax1)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    fig.colorbar(im1, cax=cax)

    if t is not None:
        if isinstance(t, str):
            text = t
        elif isinstance(t, float):
            text = r"$t = ${0:.3f}".format(t)
        else:
            raise ValueError("Got weird data type for label (t). t=", t, "type(t)=", type(t))
        plt.figtext(0.05, 0.95, text)

    return fig







def plot_2D(x, rho, u, p, t = None, nx = 100, kwargs={}):
    """
    Create a plot from 2D data.
    rho, u, p:   np arrays of physical quantities
    t:           time of the simulation
    nx:          how many bins to use to create image

    kwargs get passed to matplotlib.pyplot.imshow(), and need to be a dictionnary

    returns:
        fig:     pyplot.figure() object containing the plots
    """


    # do y-x histograms for imshow.
    rhoH, _, _ = np.histogram2d(x[:,1], x[:,0], bins=nx, range=[[0, 1], [0, 1]], weights=rho)
    uxH , _, _ = np.histogram2d(x[:,1], x[:,0], bins=nx, range=[[0, 1], [0, 1]], weights=u[:,0])
    uyH , _, _ = np.histogram2d(x[:,1], x[:,0], bins=nx, range=[[0, 1], [0, 1]], weights=u[:,1])
    pH  , _, _ = np.histogram2d(x[:,1], x[:,0], bins=nx, range=[[0, 1], [0, 1]], weights=p)


    fig = plt.figure(figsize=(21, 5))

    ax1 = fig.add_subplot(1, 4, 1)
    im1 = ax1.imshow(rhoH,
            origin='lower', 
            extent=(0,1,0,1),
             **kwargs,
            )
    ax1.set_title('density')

    ax2 = fig.add_subplot(1, 4, 2)
    im2 = ax2.imshow(uxH,
            origin='lower', 
            extent=(0,1,0,1),
             **kwargs,
            )
    ax2.set_title('velocity in x direction')

    ax3 = fig.add_subplot(1, 4, 3)
    im3 = ax3.imshow(uyH,
            origin='lower', 
            extent=(0,1,0,1),
             **kwargs,
            )
    ax3.set_title('velocity in y direction')

    ax4 = fig.add_subplot(1, 4, 4)
    im4 = ax4.imshow(pH, 
            origin='lower', 
            extent=(0,1,0,1),
             **kwargs,
            )
    ax4.set_title('pressure')

    for ax in fig.axes:
        ax.set_xlabel("x")
        ax.set_xlim(0,1)
        ax.set_ylabel("y")
        ax.set_ylim(0,1)


    for im, ax in [(im1, ax1), (im2, ax2), (im3, ax3), (im4, ax4)]:
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        fig.colorbar(im, cax=cax)


    if t is not None:
        if isinstance(t, str):
            text = t
        elif isinstance(t, float):
            text = r"$t = ${0:.3f}".format(t)
        else:
            raise ValueError("Got weird data type for label (t). t=", t, "type(t)=", type(t))
        plt.figtext(0.05, 0.95, text)

    return fig






def plot_2D_profile(x, rho, u, p, t = None, nx = 100, kwargs={}):
    """
    Create a plot from 2D data.
    rho, u, p:   np arrays of physical quantities
    t:           time of the simulation
    nx:          how many bins to use to create image

    kwargs get passed to matplotlib.pyplot.errorbar(), and need to be a dictionnary

    returns:
        fig:     pyplot.figure() object containing the plots
    """

    X = x[:,0]

    rhoM,   bin_edges, binnumber = stats.binned_statistic(X, rho,    bins=nx, statistic='mean', range=(0,1))
    rhoErr, bin_edges, binnumber = stats.binned_statistic(X, rho,    bins=nx, statistic='std',  range=(0,1))
    uxM,    bin_edges, binnumber = stats.binned_statistic(X, u[:,0], bins=nx, statistic='mean', range=(0,1))
    uxErr,  bin_edges, binnumber = stats.binned_statistic(X, u[:,0], bins=nx, statistic='std',  range=(0,1))
    uyM,    bin_edges, binnumber = stats.binned_statistic(X, u[:,1], bins=nx, statistic='mean', range=(0,1))
    uyErr,  bin_edges, binnumber = stats.binned_statistic(X, u[:,1], bins=nx, statistic='std',  range=(0,1))
    pM,     bin_edges, binnumber = stats.binned_statistic(X, p,      bins=nx, statistic='mean', range=(0,1))
    pErr,   bin_edges, binnumber = stats.binned_statistic(X, p,      bins=nx, statistic='std',  range=(0,1))


    X = 0.5*(bin_edges[:-1] + bin_edges[1:])

    fig = plt.figure(figsize=(21, 5))

    ax1 = fig.add_subplot(1, 4, 1)
    im1 = ax1.errorbar(X, rhoM, yerr=rhoErr,
             **kwargs,
            )
    ax1.set_title('density')

    ax2 = fig.add_subplot(1, 4, 2)
    im2 = ax2.errorbar(X, uxM, yerr=uxErr, 
             **kwargs,
            )
    ax2.set_title('velocity in x direction')

    ax3 = fig.add_subplot(1, 4, 3)
    im3 = ax3.errorbar(X, uyM, yerr=uyErr,
             **kwargs,
            )
    ax3.set_title('velocity in y direction')

    ax4 = fig.add_subplot(1, 4, 4)
    im4 = ax4.errorbar(X, pM, yerr=pErr,
             **kwargs,
            )
    ax4.set_title('pressure')

    for ax in fig.axes:
        ax.set_xlabel("x")
        ax.set_xlim(0,1)
        ax.set_ylabel("y")
        ax.set_ylim(0,1)


    if t is not None:
        if isinstance(t, str):
            text = t
        elif isinstance(t, float):
            text = r"$t = ${0:.3f}".format(t)
        else:
            raise ValueError("Got weird data type for label (t). t=", t, "type(t)=", type(t))
        plt.figtext(0.05, 0.95, text)

    return fig






def scatter_2D(x, rho, u, p, t = None, nx = 100, kwargs={}):
    """
    Create a scatter plot from 2D data.
    rho, u, p:   np arrays of physical quantities
    t:           time of the simulation
    nx:          how many bins to use to create image

    kwargs get passed to matplotlib.pyplot.scatter(), and need to be a dictionnary

    returns:
        fig:     pyplot.figure() object containing the plots
    """

    X = x[:,0]
    Y = x[:,1]

    alpha = 0.5

    fig = plt.figure(figsize=(21, 5))

    ax1 = fig.add_subplot(1, 4, 1)
    im1 = ax1.scatter(X, Y, c=rho,
            alpha = alpha,
             **kwargs,
            )
    ax1.set_title('density')

    ax2 = fig.add_subplot(1, 4, 2)
    im2 = ax2.scatter(X, Y, c=u[:,0],
            alpha = alpha,
             **kwargs,
            )
    ax2.set_title('velocity in x direction')

    ax3 = fig.add_subplot(1, 4, 3)
    im3 = ax3.scatter(X, Y, c=u[:,1],
            alpha = alpha,
             **kwargs,
            )
    ax3.set_title('velocity in y direction')

    ax4 = fig.add_subplot(1, 4, 4)
    im4 = ax4.scatter(X, Y, c=p,
            alpha = alpha,
             **kwargs,
            )
    ax4.set_title('pressure')

    for ax in fig.axes:
        ax.set_xlabel("x")
        ax.set_xlim(0,1)
        ax.set_ylabel("y")
        ax.set_ylim(0,1)


    for im, ax in [(im1, ax1), (im2, ax2), (im3, ax3), (im4, ax4)]:
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        fig.colorbar(im, cax=cax)


    if t is not None:
        if isinstance(t, str):
            text = t
        elif isinstance(t, float):
            text = r"$t = ${0:.3f}".format(t)
        else:
            raise ValueError("Got weird data type for label (t). t=", t, "type(t)=", type(t))
        plt.figtext(0.05, 0.95, text)

    return fig







def save_plot(fig, fname = None, case = None, plottype = 'image', fname_force = None):
    """
    Save the figure. If fname and case are given, it will first generate
    a descriptive figure name. Otherwise, it will save it as 'hydro-plot.png'

    case: overplotted, etc. See get_figname() function for details
    plottype: image, scatter, profile
    """

    if fname_force is not None:
        plt.savefig(fname_force, fig = fig)
        print("Saved figure", fname_force)

    else:

        if fname is not None:

            ax = fig.axes[0]
            if len(ax.get_lines()) > 1:
                # if we have more than one line, this figure is used to overplot things.
                # in that case, give it a different name.
                if case is None:
                    case = "overplotted"
                else:
                    # Note: this skips the case "not-overplotted", which is as intended
                    if "overplotted" not in case:
                        case += "-overplotted"
            fname = get_figname(fname, case, plottype)
        else:
            fname = 'hydro-plot.png'

        plt.savefig(fname, fig=fig)
        print("Saved figure", fname)

    plt.close()
    return
    




def get_figname(fname, case = None, plottype = 'image'):
    """
    Generate figure name using initial filename fname.
    Remove the file suffix, if present, and add a png.

    if case is not None, it will add something to the file name
    so it will be distinguishable. 
    Accepted cases:
        "density":              for density-only plotting
        "overplotted":          mutliple plots on one axis
        "density-overplotted":  density only with multiple lines per axis   

    returns:
        figname:    figure name string
    """

    # start from last letter, look for a dot to find the suffix
    # if you reach a slash first, stop there
    nchars = len(fname)
    dotindex = None
    for c in range(nchars):
        if fname[nchars-c-1] == '/':
            break
        if fname[nchars-c-1] == ".":
            dotindex = nchars - c - 1
            break

    # now extract the actual file basename
    if dotindex is None:
        figname = fname
    else:
        figname = fname[:dotindex]

    if case is not None:
        if case == "density":
            figname += "-density-only"
        if case == "overplotted":
            # first remove snapshot number
            figname = figname[:-5]
            figname += "-overplotted"
        if case == "density-overplotted":
            figname = figname[:-5]
            figname += "-density-only-overplotted"
        if case == "3D":
            figname += "-3D"
        if case == "riemann-solver":
            figname += "-riemann-solution"
        if case == "not-overplotted":
            pass
        if case == "artsy":
            figname += "-artsy"

    if plottype == 'image':
        pass
    elif plottype == 'scatter':
        figname += '-scatter'
    elif plottype == 'profile':
        figname += '-profile'
 

    figname += "." + file_format

    return figname
