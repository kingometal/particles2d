# particles2d
Simulation of round 2d particles interacting via various forces.

# Description
This is a playground project aimed to visualize the movement of particles/spheres/planets when different forces are applied on them.

The particles have a mass, a charge and a radius.

The forces which can be applied are the gravitaional, electromagnetic and the atomic force (Lennard-Jones potential).
Each force can be disabled by setting the according constand (e.g. Gravitational constant) to zero.

When the particles hit the wall, they bounce like billiard balls, similar to gas molecules in a constant volume.

# Particles overlap
If particles hit each other, they bounce like billiard balls to prevent overlap

There is a protection algorithm to prevent particles from overlapping:
If after a simulation step some particles still overlap, they are moved slightly so that they do not overlap anymore, but touch each other instead.
This behaviour is strongly non-physical, but it is required to prevent particles from accelerating to "near-infinite" speeds.
Since the atomic force was introduced, this protection algorithm can be omitted, if the force and radius parameters are set right.

The visible radius of a particle may differ from the radius used for calculations.


# Run-time controls
There is a way to control the physical constants (i.e. gravitational/electric/atomic) during the simulation via hotkeys.

There is no way to control the position or speed of a particle during the simulation.

Particles can be removed by right-clicking on them

Particles can be added by left-clicking somewhere on the screen, where no other particles are


# Prerequisites
Following libraries are required to compile the library on ubuntu 18.04:

make

g++

libsdl2-dev

libsdl2-ttf-dev 

libconfig-dev

# Compiliing
Simply run "make" in the project folder


# Running
After successful compiling, run "partices2d.x", or compile and run with "make run"


# Configuration
A configuration file is provided: Particles.cfg. 
There you can set various simulation parameters.

No command-line parameters are supported yet.


# Experimental: Compile and Run in docker
Assuming you know, what docker is:
If you prefer running the programm in a docker container instead of installing the required libraries on your pc, 
you can try running execDockerContainer.sh with the command you would like to execute, e.g.

./execDockerContainer make

or

./execDockerContainer "make run"


Required files are runDocker.sh and Dockerfile
