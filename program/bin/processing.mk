# Makefile to be included by other makefiles where the definitions and variables are processed.
# Also has some global definitions, like srcdir and vpath.
# Not intended to be used by itself!
# Mainly used so all the different makefiles can just import it and I don't have to keep track
# of what I updated where.


#==============================
# Preprocessing defines
#==============================

#----------------------------------------------------
# set default values if not defined otherwise
#----------------------------------------------------

ifndef NDIM
NDIM = 1
endif

ifndef SOLVER
SOLVER = SPH-DS
endif

ifndef KERNEL
KERNEL = CUBIC-SPLINE
endif

ifndef RIEMANN
RIEMANN = NONE
endif

ifndef LIMITER
LIMITER = NONE
endif

ifndef SOURCES
SOURCES = NONE
endif









#----------------------------------------------------
# Do some checks
#----------------------------------------------------

# make sure Riemann solvers are selected if 
# hydro is being solved

ifeq ($(strip $(SOLVER)), SPH_DS)
SPH = true
endif

ifeq ($(strip $(SOLVER)), MESHLESS)
MESHLESS = true
endif

ifeq ($(strip $(SOLVER)), MESHLESS_IVANOVA)
MESHLESS = true
endif

ifdef $(MESHLESS)
ifeq ($(strip $(RIEMANN)), NONE)
RIEMANN = EXACT
endif
endif


# make sure an integrator is selected if
# sources are selected
ifneq ($(strip $(SOURCES)), NONE)
ifndef INTEGRATOR
INTEGRATOR = RK2
endif
ifeq ($(strip $(INTEGRATOR)), NONE)
INTEGRATOR = RK2
endif
else # if SOURCES = NONE
INTEGRATOR = NONE
endif









# ------------------------------------------------
# transform defines into integers where needed
# ------------------------------------------------

ifeq ($(strip $(SOLVER)), SPH_DS)
SOLVERINT = 11
endif
ifeq ($(strip $(SOLVER)), MESHLESS)
SOLVERINT = 21
endif
ifeq ($(strip $(SOLVER)), MESHLESS_IVANOVA)
SOLVERINT = 22
endif


ifeq ($(strip $(RIEMANN)), NONE)
RIEMANNINT = 0
endif
ifeq ($(strip $(RIEMANN)), EXACT)
RIEMANNINT = 1
endif
ifeq ($(strip $(RIEMANN)), HLLC)
RIEMANNINT = 2
endif
ifeq ($(strip $(RIEMANN)), TRRS)
RIEMANNINT = 3
endif
ifeq ($(strip $(RIEMANN)), TSRS)
RIEMANNINT = 4
endif


ifeq ($(strip $(LIMITER)), NONE)
LIMITERINT = 0
endif
ifeq ($(strip $(LIMITER)), MINMOD)
LIMITERINT = 1
endif
ifeq ($(strip $(LIMITER)), SUPERBEE)
LIMITERINT = 2
endif
ifeq ($(strip $(LIMITER)), VANLEER)
LIMITERINT = 3
endif
ifeq ($(strip $(LIMITER)), MC)
LIMITERINT = 4
endif



ifeq ($(strip $(SOURCES)), NONE)
SOURCESINT = 0
endif
ifeq ($(strip $(SOURCES)), CONSTANT)
SOURCESINT = 1
endif
ifeq ($(strip $(SOURCES)), RADIAL)
SOURCESINT = 2
endif





#-----------------------------------
# Set up Definition Flags
#-----------------------------------

COMPILEDATE:=$(shell date "+%F %T")


DEFINES= -DNDIM=$(NDIM) -DSOLVER=$(SOLVERINT) -DRIEMANN=$(RIEMANNINT) -DLIMITER=$(LIMITERINT) -DSOURCE=$(SOURCESINT) -DCOMPDATE="$(COMPILEDATE)" 


ifdef SPH
DEFINES += -DSPH
endif

ifdef MESHLESS
DEFINES += -DMESHLESS
endif

ifneq ($(strip $(SOURCES)), NONE)
DEFINES += -DWITH_SOURCES
endif





#==================================
# FILE LISTS
#==================================

SRCDIR=../src

#include paths. Will be followed in that order.
# VPATH=$(SRCDIR):$(SRCDIR)/limiter:$(SRCDIR)/solver:$(SRCDIR)/riemann:$(SRCDIR)/sources:$(SRCDIR)/integrate
VPATH=$(SRCDIR):$(SRCDIR)/solver

#include directories for headers
IDIR=$(SRCDIR)


ifeq ($(strip $(SOLVER)), SPH_DS)
	HYDROOBJ=SPH-density-entropy.o
endif
ifeq ($(strip $(SOLVER)), MESHLESS)
	HYDROOBJ=meshless.o
endif
ifeq ($(strip $(SOLVER)), MESHLESS_IVANOVA)
	HYDROOBJ=meshless.o
endif


ifeq ($(strip $(LIMITER)), NONE)
	LIMITEROBJ=no_limiter.o
endif
ifeq ($(strip $(LIMITER)), MINMOD)
	LIMITEROBJ=minmod.o
endif
ifeq ($(strip $(LIMITER)), SUPERBEE)
	LIMITEROBJ=superbee.o
endif
ifeq ($(strip $(LIMITER)), VANLEER)
	LIMITEROBJ=van_leer.o
endif
ifeq ($(strip $(LIMITER)), MC)
	LIMITEROBJ=monotonized_central_difference.o
endif


ifeq ($(strip $(RIEMANN)), NONE)
	RIEMANNOBJ=
endif
ifeq ($(strip $(RIEMANN)), EXACT)
	RIEMANNOBJ=riemann-exact.o riemann.o
endif
ifeq ($(strip $(RIEMANN)), TRRS)
	RIEMANNOBJ=riemann-trrs.o riemann.o
endif
ifeq ($(strip $(RIEMANN)), TSRS)
	RIEMANNOBJ=riemann-tsrs.o riemann.o
endif
ifeq ($(strip $(RIEMANN)), HLLC)
	RIEMANNOBJ=riemann-hllc.o riemann.o
endif



ifeq ($(strip $(SOURCES)), NONE)
	SRCOBJ=
endif
ifeq ($(strip $(SOURCES)), CONSTANT)
	SRCOBJ=sources.o sources-constant.o
endif
ifeq ($(strip $(SOURCES)), RADIAL)
	SRCOBJ=sources.o sources-radial.o
endif



ifeq ($(strip $(INTEGRATOR)), NONE)
	INTOBJ=
endif
ifeq ($(strip $(INTEGRATOR)), RK2)
	INTOBJ=integrate-runge-kutta-2.o
endif
ifeq ($(strip $(INTEGRATOR)), RK4)
	INTOBJ=integrate-runge-kutta-4.o
endif






# OBJECTS = main.o gas.o params.o particles.o io.o utils.o cell.o solver.o limiter.o $(HYDROOBJ) $(LIMITEROBJ) $(RIEMANNOBJ) $(SRCOBJ) $(INTOBJ)
OBJECTS = main.o gas.o params.o particles.o io.o utils.o cell.o solver.o $(HYDROOBJ)
