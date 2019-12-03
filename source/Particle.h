#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"
#include <map>

class ForceStruct
{
public:
    ForceStruct()
        : F(0.0, 0.0)
        , Updated(false)
    {
    }

    Vector F;
    bool Updated;
};

class Particle
{
public:
    Particle();

    Vector Position;
    Vector Velocity;
    Vector Force;
    std::map<int, ForceStruct> Forces;

    Vector GetFullForce();

    double Mass;
    double ReciMass;
    double Charge;
    double Radius;
};
#endif
