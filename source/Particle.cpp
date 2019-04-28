#include "Particle.h"
#include <cmath>

Particle::Particle(void)
    : Position()
    , Velocity()
    , Force()
    , Mass(1.0)
    , ReciMass(1.0 / Mass)
    , Charge(0.0)
    , Radius(1.0)
{
}
