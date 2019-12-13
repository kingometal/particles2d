#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
public:
    Particle();
    ~Particle() = default;

    Vector Position;
    Vector Velocity;

    double Mass;
    double Charge;
    double Radius;
};
#endif
