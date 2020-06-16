#!/usr/bin/env python3

#----------------------------
# Various small utilities
#----------------------------


from sys import argv
import os

def get_only_cmdlinearg():
    """
    Get the only expected cmdline argument. If there is none
    or too many, throw error.
    """

    argc = len(argv)
    if argc != 2:
        print(argc, argv)
        raise ValueError("I expect exactly one argument: The filename.")
    else:
        return argv[1]



def get_all_files_with_same_basename(fname):
    """
    Get a list of all files with the same basename as given file <fname>.
    Basename in this case means everything before _XXXX.out, which is the
    format of the hydro output files.
    """

    # first generate basename
    basename = fname[:-9] # remove -0000.out
    
    basedir = os.path.dirname(basename)
    if basedir == '':
        basedir = os.getcwd()

    allfiles = os.listdir(basedir)

    filelist = []

    for f in allfiles:
        if f.startswith(basename) and f.endswith(".out"):
            filelist.append(f)

    filelist.sort()

    return filelist



def label_to_kwargs(t, kwargs={}):
    """
    Generate a label used in matplotlib.pyplot.plot() or similiar using either
        - a string t: just pass it on as the label string
        - a float t:  format it first: "t = {0:.3f}".format(t)
    and add it to the kwargs dictionnary
    
    returns:
        kwargs dictionnary
    """

    text = None
    if isinstance(t, str):
        text = t
    elif isinstance(t, float):
        text = r"$t = ${0:.3f}".format(t)
    else:
        raise ValueError("Got weird data type for label (t). t=", t, "type(t)=", type(t))

    if text is not None:
        kwargs["label"] = text

    return kwargs



def parse_arguments():
    """
    Parse arguments.
        -i, --image:    make an image [default]
        -p, --profile:  plot profiles
        -s, --scatter:  make scatterplot

    returns:
        case:   "image", "profile", or "scatter" string
        filelist:   list of files given
    """


    import argparse

    parser = argparse.ArgumentParser(description="Particle Hydro Plotting Script")
    parser.add_argument('inputfiles', metavar='filenames', type=str, nargs='+', help="file(s) to plot")
    parser.add_argument('-i', '--image',   dest='image',   action='store_true', help='make an image [default]. For 1D data, this creates a scatterplot.')
    parser.add_argument('-p', '--profile', dest='profile', action='store_true', help='plot profiles. For 1D data, this has no effect.')
    parser.add_argument('-s', '--scatter', dest='scatter', action='store_true', help='make a scatterplot. For 1D data, this has no effect.')

    args = parser.parse_args()

    if args.image or args.profile or args.scatter:
        # at least one is true. Now check that it's exactly one
        if args.image and args.profile:
            print("You set to plot images and profiles. Choose only one")
            quit()
        if args.image and args.scatter:
            print("You set to plot images and scatterplots. Choose only one")
            quit()
        if args.profile and args.scatter:
            print("You set to plot profiles and scatterplots. Choose only one")
            quit()

        # find case
        if args.image:
            case = "image"
        if args.profile:
            case = "profile"
        if args.scatter:
            case = "scatter"
    else:
        # none given. Use default
        case = "image"


    return case, args.inputfiles
