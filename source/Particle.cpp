#include "Particle.h"
#include <cmath>
#include <iostream>

Particle::Particle(void)
    : Position()
    , Velocity()
    , Force()
    , Mass(1.0)
    , ReciMass(1.0 / Mass)
    , Charge(0.0)
    , Radius(1.0)
    , Forces()
{
}

Vector Particle::GetFullForce()
{
//    return Force;
    Vector sum (0,0);
    for (auto i : Forces)
    {
        sum += i.second.F;
        Forces[i.first].Updated = false;
    }
    return sum;
}
