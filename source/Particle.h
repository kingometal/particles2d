#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"
#include <map>

class Particle
{
public:
    Particle();

    Vector Position;
    Vector Velocity;

    double Mass;
    double ReciMass;
    double Charge;
    double Radius;
};
#endif
