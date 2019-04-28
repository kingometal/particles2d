#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
public:
    Particle();

    Vector Position;
    Vector Velocity;
    Vector Force;

    double Mass;
    double ReciMass;
    double Charge;
    double Radius;
};
#endif
