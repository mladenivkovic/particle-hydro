Particle-based Hydro Solvers
====================================


Description
-----------------------

A program to learn about and play with particle based hydrodynamics solvers.
You can pick between:

- Smooth Particle Hydrodynamics (SPH) Flavours:
	- Density-Entropy SPH
    - Meshless methods of Vila
    - Meshless method of Ivanova 2014
- Riemann solvers (for Meshless Methods):
	- Exact	(exact iterative solver)
	- HLLC	(Harten, Lax, van Leer approximate solver with contact wave)
	- TRRS	(Two Rarefaction approximate Riemann Solver)
	- TSRS	(Two Shock approximate Riemann Solver)
- Limiters:
    - no limiter
    - minmod
    - superbee
    - monotonized centered difference (MC)
    - van Leer
- Number of dimensions:
	- 1, 2, or 3
- Source terms:
    - constant cartesian
    - constant radial w.r.t. box center
- Integrators (for source terms):
    - Runge Kutta 2
    - Runge Kutta 4

at compile time by setting the corresponding values in the Makefile.

To see what the code is able to do, you can for example run the test script in `./program/test` and have a look at the resulting `./program/test/test_results.pdf`
(You'll need to add add `/py/module/` to your `PYTHONPATH` to be able to run the python plotting scripts.)
Another option is to just run `./program/bin/run.sh` to run a Kelvin-Helmholtz instability simulation.

You can read up how the code does that with the tex and pdf files in `tex/`.

The purpose of this project is to learn about, play with, and eventually teach the basics of (finite volume) fluid dynamics.
With that in mind, the project is deliberately written to be easy to read, and well documented (check out `/tex/equations/equations_and_implementation_details.pdf` !)
So things are kept simple and non-optimized.





Contents
----------------------------

- `./IC`: A collection of default initial condition files.
- `./program`: contains the actual software. The source code is in `./program/src`, the Makefile in `./program/bin` 
- `./py`: python scripts for visualisation, generating ICs, etc.
- `./sh`: some bash scripts to run multiple jobs in a coordinated fashion, produce comparisons etc.
- `./tex`: TeX documents containing every equations that is used, some implementation details, etc., some results and conclusions.





Requirements
----------------------

- A good old C compiler. Code is written in C11 standard. I only tested it with `gcc 8.3.0` though. 
- GNU make to compile without much hassle. The Makefile is set up for GCC.
- python 3 with numpy and matplotlib for plotting. 
- LaTeX to create the TeX files. I hardcoded the `pdflatex` command in the scripts. It doesn't require any fancy LaTeX packages.





Usage
------------

```
cd program/bin/
make
./hydro paramfile ic-file
```

An example parameter file is given in `bin/example-paramfile.txt`.
Example IC files are in `IC/`


```
cd program/bin/
make -f Makefile-Riemann
./riemann paramfile ic-file
```
in the parameter file, you need to specify `nx` and `tmax` only, pretty much all other parameters are ignored.








Things to keep in mind
----------------------------

- General:
    - If you want to use the included python modules and scripts, you need to add `/py/module/` to your `PYTHONPATH`.
    - All macros are in `defines.h`
    - Boxsize is always assumed to be 1 (in any dimension), and starts at zero. Can be changed in `defines.h` though.
    - There is only a uniform grid. If you want AMR or non-Cartesian geometries, do it yourself.
- Hydrodynamics related:
    - This code is written assuming ideal gasses with adiabatic index gamma = 5/3. If you want something different, change it in `defines.h`
    - If you change the adiabatic index in the definitions, and want to overplot exact Riemann solutions using the python scripts, don't forget to change gamma in `py/module/hydro_riemann.py` as well!
- Source terms:
    - Source terms have only been implemented for hydro applications. It should be straightforward to add them to advection though.
- Integrators:
    - Integrators are only employed if there are source terms to add to the Euler equations.








Parameter File
---------------------

The parameter file is written in plain text in the format
```
/* this is a comment and is ignored in the parameter file */
// this type of comment is ignored too

/* empty lines, like the one above and below this line, are ignored too */

<name> = <value>
```





Accepted names, their datatypes and default values are:


### Behaviour Options and Parameters

| name          |  default value    | type  | description                                                                   |
|---------------|-------------------|-------|-------------------------------------------------------------------------------|
| `verbose`     | = 0               | `int` | How talkative the code should be. 0 = quiet, 1 = talky, 2 = no secrets, 3 = debugging        |
|               |                   |       |                                                                               |
| `nstep_log`   | = 0               | `int` | Write log messages only ever `nstep_log` steps. If 0, will write every step.  |
|               |                   |       |                                                                               |



### Simulation Related Options and Parameters

| name              |  default value    | type  | description                                                                   |
|-------------------|-------------------|-------|-------------------------------------------------------------------------------|
| `ccfl`            | = 0.9             |`float`| courant factor; `dt = ccfl * dx / vmax`                                       |
|                   |                   |       |                                                                               |
| `nsteps`          | = 1               | `int` | Up to how many steps to do. If = 0, run until `t >= tmax`                     |
|                   |                   |       |                                                                               |
| `tmax`            | = 0               |`float`|  Up to which time to simulate. If `nsteps` is given, will stop running if `nsteps` steps are reached before `tmax` is.     |
|                   |                   |       |                                                                               |
| `force_dt`        | = 0               |`float`| force a time step size. If a smaller time step is required, the sim will stop.|
|                   |                   |       |                                                                               |
| `boundary`        | = 0               | `int` | Boundary conditions  0: periodic. 1: transmissive. This sets the boundary conditions for all walls. |
|                   |                   |       |                                                                               |



### Output related Options and Parameters


| name          |  default value    | type    | description                                                                   |
|---------------|-------------------|---------|-------------------------------------------------------------------------------|
| `foutput`     | = 0               | `int`   | Frequency of writing outputs in number of steps. If = 0, will only write initial and final steps.  |
|               |                   |         |                                                                               |
| `dt_out`      | = 0               |`float`  | Frequency of writing outputs in time intervals. Code will always write initial and final steps as well.  |
|               |                   |         |                                                                               |
| `toutfile`    | None              |`string` | File name containing desired times (in code units) of output. Syntax of the file: One float per line with increasing value.  |
|               |                   |         |                                                                               |
| `basename`    | None              |`string` | Basename for outputs.  If not given, a basename will be generated based on compilation parameters and IC filename.       |
|               |                   |         |                                                                               |



### Source Term related Options and Parameters

The source term related options will only take effect if the code has been compiled to add source terms.


| name              |  default value    | type   | description                                                                   |
|-------------------|-------------------|--------|-------------------------------------------------------------------------------|
| `src_const_acc_x` | = 0               | `float`| constant acceleration in x direction for constant source terms                |
|                   |                   |        |                                                                               |
| `src_const_acc_y` | = 0               | `float`| constant acceleration in y direction for constant source terms                |
|                   |                   |        |                                                                               |
| `src_const_acc_r` | = 0               | `float`| constant acceleration in radial direction for radial source terms             |
|                   |                   |        |                                                                               |


Initial Conditions
------------------------------------

- The program reads formatted text Initial Conditions (ICs).
- It is assumed that the particle quantities are the fluid quantities at the initial time.
- Lines starting with `//` or `/*` will be recognized as comments and skipped. Empty lines are skipped as well.
- Some example python scripts that generate initial conditions are given in `./py/IC`



You can provide an individual value for mass, position, velocity, and pressure for each cell. The IC file format is:

The lines
```
ndim = <int>
npart = <int>
```

**must** be the first non-comment non-empty lines, in that order.

**FOR 1D:**

```
ndim = 1
npart = <integer, number of particles in total>
<mass of particle 0> <position of particle 0> <velocity of particle 0> <pressure of particle 0>
<mass of particle 1> <position of particle 1> <velocity of particle 1> <pressure of particle 1>
                                    .
                                    .
                                    .
<mass of particle npart-1> <position of particle npart-1> <velocity of particle npart-1> <pressure of particle npart-1>
```

All values for mass, positions, velocity, and pressure must be floats.
You can put comments and empy lines wherever you feel like it.






**FOR 2D:**

```
ndim = 2
npart = <integer, number of cells in any dimension>
<mass of particle 0> <x position of particle 0> <y position of particle 0> <x velocity of particle 0> <y velocity of particle 0> <pressure of particle 0>
<mass of particle 1> <x position of particle 1> <y position of particle 1> <x velocity of particle 1> <y velocity of particle 1> <pressure of particle 1>
                                    .
                                    .
                                    .
<mass of particle npart-1> <x position of particle npart-1> <y position of particle npart-1> <x velocity of particle npart-1> <y velocity of particle npart-1> <pressure of particle npart-1>

```

All values for mass, position, velocity, and pressure must be floats.
You can put comments and empy lines wherever you feel like it.










Output
---------------------


If no `basename` is given in the parameter file, the output file name will be generated as follows:

`<ICfile-without-suffix>-<SOLVER>-<NDIM>D-<snapshot nr>.out`

e.g.

`run-SPH_DS-2D-0001.out`

The output files are written in formatted text, and their content should be self-explainatory:




**FOR 1D**:

```
# ndim =  1
# nx =    <number of cells used>
# t =     <current time, float>
# nsteps =  <current step of the simulation>
#            x          rho            u            p
< x value of cell 0 > <density of cell 0> <velocity of cell 0> < pressure of cell 0>
                            .
                            .
                            .
                            .
< x value of cell nx-1 > <density of cell nx-1> <velocity of cell nx-1> < pressure of cell nx-1>
```





**FOR 2D:**

```
# ndim =  2
# nx =    <number of cells used>
# t =     <current time, float>
# nsteps =  <current step of the simulation>
#            x            y          rho          u_x          u_y            p
<x value of cell (0, 0)> <y value of cell (0, 0)> <density in cell (0, 0)> <x velocity in cell (0, 0)> <y velocity in cell (0, 0)> <pressure in cell (0, 0)>
<x value of cell (1, 0)> <y value of cell (1, 0)> <density in cell (1, 0)> <x velocity in cell (1, 0)> <y velocity in cell (1, 0)> <pressure in cell (1, 0)>
                                                 .
                                                 .
                                                 .
<x value of cell (nx-1, 0)> <y value of cell (nx-1, 0)> <density in cell (nx-1, 0)> <x velocity cell (nx-1, 0)> <y velocity in cell (nx-1, 0)> <pressure in cell (nx-1, 0)>
<x value of cell (0, 1)> <y value of cell (0, 1)> <density in cell (0, 1)> <x velocity in cell (0, 1)> <y velocity in cell (0, 1)> <pressure in cell (0, 1)>
<x value of cell (1, 1)> <y value of cell (1, 1)> <density in cell (1, 1)> <x velocity in cell (1, 1)> <y velocity in cell (1, 1)> <pressure in cell (1, 1)>
                                                 .
                                                 .
                                                 .
<x value of cell (nx-1, 1)> <y value of cell (nx-1, 1)> <density in cell (nx-1, 1)> <x velocity cell (nx-1, 1)> <y velocity in cell (nx-1, 1)> <pressure in cell (nx-1, nx-1)>
                                                 .
                                                 .
                                                 .
<x value of cell (0, nx-1)> <y value of cell (0, nx-1)> <density in cell (0, nx-1)> <x velocity in cell (0, nx-1)> <y velocity in cell (0, nx-1)> <pressure in cell (0, nx-1)>
<x value of cell (1, nx-1)> <y value of cell (1, nx-1)> <density in cell (1, nx-1)> <x velocity in cell (1, nx-1)> <y velocity in cell (1, nx-1)> <pressure in cell (1, nx-1)>
                                                 .
                                                 .
                                                 .
<x value of cell (nx-1, nx-1)> <y value of cell (nx-1, nx-1)> <density in cell (nx-1, nx-1)> <x velocity cell (nx-1, nx-1)> <y velocity in cell (nx-1, nx-1)> <pressure in cell (nx-1, nx-1)>
```







Visualisation
-----------------------------------

Some basic scripts to visualize ICs and outputs are given in the `./py/plotting` directory. See the `README.md` in the `./py` directory for more details.




Tinkering with the Code
----------------------------------

I tried keeping the code as modular as possible, so adding/removing stuff should work fine. If you want to do that, here's a list of functions that could be useful:

### in `utils.h`:

- `log_extra()`, `debugmessage()`, `throw_error()`: Functions that will print to stdout depending on the level of verbosity you set in the paramfile.
- `printbool()`: print boolean as true or false to stdout.




### in `cell.h`:

- `cell_print_grid()`: prints out chosen grid quantity for the entire grid to stdout. Works independently of dimension of the code, so you can always call it.
- `cell_print_grid_part()`: prints out chosen grid quantity for chosen cell intervals, i.e. give it xmin and xmax etc. 






Contributing
-----------------------------------

Yes please!
