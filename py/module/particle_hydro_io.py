#!/usr/bin/env python3

#-------------------------------------------------------
# Module that contains io-routines for the hydro outputs
# and IC files.
#-------------------------------------------------------


import numpy as np




def write_ic(fname, ndim, x, m, u, p):
    """
    Write an (arbitrary type) IC file.
    fname:  filename to be written
    ndim:   number of dimensions
    x:      numpy array for position
    m:      numpy array for mass
    u:      numpy array for velocity
    p:      numpy array for pressure

    returns:
        Nothing
    """

    f = open(fname, 'w')
    npart = m.shape[0]

    f.write("ndim = {0:d}\n".format(ndim))
    f.write("npart = {0:d}\n".format(npart))
    f.write("\n") # add newline

    if ndim == 1:
        for i in range(npart):
            f.write("{0:12.6f} {1:12.6f} {2:12.6f} {3:12.6f}\n".format(x[i], m[i], u[i], p[i]))

    elif ndim == 2:
        for i in range(npart):
            f.write("{0:12.6f} {1:12.6f} {2:12.6f} {3:12.6f} {4:12.6f} {5:12.6f}\n".format(
                x[i,0], x[i, 1], m[i], u[i,0], u[i, 1], p[i])
            )

    return






def read_output(fname):
    """
    Read the given output file.
    returns:
        ndim:       integer of how many dimensions we have
        x:          numpy array for positions. In 1D: is 1D array. In 2D: is 2D array containing
                    both x and y
        m:          numpy array for mass
        rho:        numpy array for density
        u:          numpy array for velocity. In 1D: is 1D array. In 2D: is 2D array containing
                    both ux and uy
        p:          numpy array for pressure
        h:          numpy array for smoothing length
        t:          time of the output
        step:       current step of the simulation
    """

    check_file_exists(fname)

    f = open(fname)

    npart = None
    ndim = None
    t = None
    step = None


    linecount = 0
    while True:
        # safety measure
        linecount += 1
        if linecount > 1000:
            print("================ Got to linecount = 1000 without having found all metadata. Wtf? Exiting now.")
            print("got npart:", npart)
            print("got ndim", ndim)
            print("got t:", t)
            print("got step:", step)
            quit(1)

        line = f.readline()
        clean = remove_python_style_comments(line)
        if line_is_empty(clean):
            continue

        else:
            if clean.strip().startswith("x"): # header name descriptions
                continue
            name, eq, value = clean.partition("=")
            nstr = name.strip()
            if nstr == "npart":
                npart = int(value)
            elif nstr == "ndim":
                ndim = int(value)
            elif nstr == "t":
                t = float(value)
            elif nstr == "nsteps":
                step = int(value)
            else:
                raise ValueError("Unknown name: '{0}'".format(name))

        if npart is not None and ndim is not None and t is not None and step is not None:
            break

 
    f.close()

    if ndim == 1:
        x, m, rho, u, p, h = np.loadtxt(fname, dtype=np.float, unpack=True, skiprows=linecount)

    elif ndim == 2:
        x, y, m, rho, ux, uy, p, h = np.loadtxt(fname, dtype=np.float, unpack=True, skiprows=linecount)

        x = np.stack((x, y), axis=1)
        u = np.stack((ux, uy), axis=1)



    return ndim, x, m, rho, u, p, h, t, step






def read_ic(fname):
    """
    Read in the given IC file. File is passed as string fname.
    It figures out the dimensions etc by itself. Returns the relevant data as numpy
    arrays, which can be either 1D or 2D, depending on IC.

    returns:
        ndim:       integer of how many dimensions we have
        x:          numpy array for positions. In 1D: is 1D array. In 2D: is 2D array containing
                    both x and y
        m:          numpy array for mass
        u:          numpy array for velocity. In 1D: is 1D array. In 2D: is 2D array containing
                    both ux and uy
        p:          numpy array for pressure
    """


    check_file_exists(fname)

    f = open(fname)
    data = f.readlines()
    f.close()

    got_npart = False
    got_ndim = False
    got_header = False

    i = 0

    # we can't just use numpy.loadtxt(). The IC files may contain comments and empty lines everywhere.
    for line in data:
        clean = remove_C_style_comments(line)
        clean = remove_newline(clean)
        if line_is_empty(clean):
            continue

        if not got_header:
            name, eq, value = clean.partition("=")
            name = name.strip()
            if name == "ndim":
                ndim = int(value)
                got_ndim = True
            elif name == "npart":
                npart = int(value)
                got_npart = True
            else:
                print("Unrecognized value name:", name)

            got_header = got_npart and got_ndim
            if got_header:
                if ndim == 1:
                    x = np.empty((npart), dtype=np.float) 
                    m = np.empty((npart), dtype=np.float)
                    u = np.empty((npart), dtype=np.float)
                    p = np.empty((npart), dtype=np.float)

                elif ndim == 2:
                    x = np.empty((npart, 2), dtype=np.float) 
                    m = np.empty((npart), dtype=np.float)
                    u = np.empty((npart, 2), dtype=np.float)
                    p = np.empty((npart), dtype=np.float)

        else:

            vals = split_columns(clean)

            if ndim == 1:
                if len(vals) != 4:
                    print("Got wrong number of values in the line.")
                    print("Line was: ", line, end='')
                    print("Cleaned line is: '{0}'".format(clean))
                    print("I expect 4 values")
                    print("I got:", len(vals), vals)
                    quit(1)

                x[i] = float(vals[0])
                m[i] = float(vals[1])
                u[i] = float(vals[2])
                p[i] = float(vals[3])
                i += 1

            elif ndim == 2:
                if len(vals) != 6:
                    print("Got wrong number of values in the line.")
                    print("Line was: ", line, end='')
                    print("Cleaned line is: '{0}'".format(clean))
                    print("I expect 6 values")
                    print("I got:", len(vals), vals)
                    quit(1)

                x[i, 0] = float(vals[0])
                x[i, 1] = float(vals[1])
                m[i] = float(vals[2])
                u[i, 0] = float(vals[3])
                u[i, 1] = float(vals[4])
                p[i] = float(vals[5])
                i += 1




    # checks
    if i != npart:
        print("Got too few values in x direction. Got i=", i, "should be", npart)
        quit(1)



    return ndim, x, m , u, p







def remove_python_style_comments(line):
    """
    Remove # if the line starts with it
    """

    if line[0] == "#":
        return line[1:].strip()

    return line







def remove_C_style_comments(line):
    """
    Remove all comments (//, /* .. */) from the line
    actually just look for a slash. It has no other business in there.
    """

    clean = line

    for i, c in enumerate(line):
        if c == '/':
            clean = line[:i]
            break

    return clean






def line_is_empty(line):
    """
    Check whether line contains only spaces and/or newline chars
    """
    clean = line.strip()
    if len(clean) == 0:
        return True
    else:
        return False







def check_file_exists(fname):
    """
    Check that file exists, throw error if not.
    """
    import os

    if not os.path.isfile(fname):
        raise ValueError("Given file {0} doesn't exist.".format(fname))
    return







def remove_newline(line):
    """
    If newline character is the last character of the line, remove it.
    """
    if len(line) > 0:
        if line[-1] == '\n' or line[-1] == '\r':
            return (line[:-1]).strip()

    return line






def split_columns(line, delim=" "):
    """
    Split given line (string) into columns defined by the delimiter delim.
    If the delimiter occurs multiple times back - to - back, treat it as
    only one column delimiter.

    returns:
        splits: List of strings, representing columns

    """


    splits = []

    start = 0
    stop = 0

    while stop < len(line) and start < len(line):
        if line[start] == delim:
            start += 1
            continue

        if line[stop] != delim:
            if line[stop] == "\n" or line[stop] == "\r":
                # add value without newline
                splits.append(line[start:stop])
                start = stop
                break
            else:
                stop += 1
                continue

        else: # if line[stop] == delim
            splits.append(line[start:stop])
            while stop < len(line):
                if line[stop] == delim:
                    stop += 1
                else:
                    if line[stop] == "\n" or line[stop] == "\r":
                        # add value without newline
                        splits.append(line[start:stop])
                        start = stop
                    break
            start = stop

    # add final column
    if start != stop:
        splits.append(line[start:])

    return splits
