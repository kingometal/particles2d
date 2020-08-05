# particles2d
Simulation of round 2d particles interacting via various forces.


# Description
This is a playground project aimed to visualize the movement of particles/spheres/planets when different forces are applied on them.

The particles have a mass, a charge and a radius.

The forces which can be applied are the gravitaional, electromagnetic and the intermolecular force (Lennard-Jones potential).
Each force can be disabled by setting the according constand (e.g. Gravitational constant) to zero.

When the particles hit the wall, they bounce like billiard balls, similar to gas molecules in a constant volume.

An option to enable the dissipation is introduced. Dissipation reduces the velocity of each particle by a factor in each simulation step.


# Particles overlap
If particles hit each other, they bounce like billiard balls to prevent overlap

There is a protection algorithm to prevent particles from overlapping:
If after a simulation step some particles still overlap, they are moved slightly so that they do not overlap anymore, but touch each other instead.
This behaviour is strongly non-physical, but it is required to prevent particles from accelerating to "near-infinite" speeds.
Since the intermolecular force was introduced, this protection algorithm can be omitted, if the force and radius parameters are set right.

The visible radius of a particle may differ from the radius used for calculations.


# Run-time controls
There is a way to control the physical constants (i.e. gravitational/electric/atomic) during the simulation via hotkeys.

## Hotkeys
- **r** resets the particles to initial positions and velocities. Physical constants and window size do not reset. The particle number is reset.
- **a** removes the last added particle. Nice way to clear the field before adding own particles.
- **NUMPAD_-** reduces the gravitational constant by factor 2. If it is 0.000000000001, sets it to 0.
- **NUMPAD_+** increases the gravitaional constant by factor 2. If it is 0, sets it to 0.000000000001.
- **NUMPAD_1** reduces the electrostatic constant by factor 2. If it is 0.000000000001, sets it to 0.
- **NUMPAD_2** increases the gravitaional constant by factor 2. If it is 0, sets it to 0.000000000001.
- **NUMPAD_4** reduces the intermolecular potential well depth by factor 2. If it is 0.000000000001, sets it to 0.
- **NUMPAD_5** increases the intermolecular potential well depth by factor 2. If it is 0, sets it to 0.000000000001.
- **i** toggles particle interaction. When interaction is disabled, particles ignore each other, like fermions (e.g. photons) do.
- **v** toggles drawing of particle velocities. Particle velocities are drawn as thin lines. Line length is proportional to the particle's velocity
- **e** toggles displaying of energies present in the system.
- **NUMPAD_/** reduces the radius of all particles by 0.1. The particle radius is only used for collision prevention, and is not visible
- **NUMPAD_\*** increases the radius of all particles by 0.1
- **NUMPAD_7** reduces the dissipation by factor 2. If it is 0.000000000001, sets it to 0.
- **NUMPAD_8** increases the dissipation by factor 2. If it is 0, sets it to 0.000000000001.


## Mouse controls

Particles can be removed by right-clicking on them.

Particles can be added by left-clicking somewhere on the screen, where no other particles are.

The window size can be changed. The particles get squieezed in the box. If you do it too fast, the simulation will most likely end up with particles at "near-infinite" speed.

## Other
There is no way to control the position or speed of a particle during the simulation.


# Prerequisites
Following libraries are required to compile the library on ubuntu 18.04:

make

g++

libsdl2-dev

libsdl2-ttf-dev 

libconfig-dev

# Compiliing
Simply run "make" in the project folder

New alternative: you can use Cmake to compile and run it. An example how to do this can be found in buildWithCMake.sh. It can be executed to build and run the program


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
