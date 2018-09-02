# particles2d
Simulation of round 2d particles interacting via various forces.

# Description
This is a playground project aimed to visualize the movement of particles/spheres/planets when different forces are applied on them.

The particles have a mass, a charge and a radius.

The forces which can be applied are the gravitaional, electromagnetic and the atomic force.
Each force can be disabled by setting the according constand (e.g. Gravitational constant) to zero.

If particles hit each other, they bounce like billiard balls to prevent overlap.
The same happens when particles hit the wall.

There is a protection algorithm to prevent particles from overlapping:
If after a simulation step some particles still overlap, they are moved slightly so that they do not overlap anymore, but touch each other instead.
This behaviour is strongly non-physical, but it is required to prevent particles from accelerating to near-infinite speeds.
Since the atomic force was introduced, this protection algorithm can be omitted, if the force and radius parameters are set right.

The visible radius of a particle may differ from the radius used for calculations.

There is no way to control the position or speed of a particle during the simulation.

There is a way to control the physical constants (i.e. gravitational/electric/atomic) during the simulation via hotkeys.



# Prerequisites
Following libraries are required to compile the library on linux (alpine:3.7):

make

g++

libx11-dev


# Compiliing
Simply run "make" in the project folder


# Running
After successful compiling, run "partices2d.x". 


# Configuration
No command-line parameters are supported yet.
Consider editing "constants.h" instead.


# Experimental: Compile and Run in docker
Assuming you know, what docker is:
If you prefer running the programm in a docker container instead of installing the required libraries on your pc, 
you can try running runDocker.sh.

Required files are runDocker.sh and Dockerfile
