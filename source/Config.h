#ifndef PARTICLES_PARAMETERS_H
#define PARTICLES_PARAMETERS_H

#include "RGBData.h"
#include "Vector.h"

class Config
{
public:
    Config(void);

    int WindowSideSizeX;
    int WindowSideSizeY;
    double GravitationalConstant;
    double ElectrostaticConstant;
    double MagneticPermeability;
    double MolecularBondingEnergy;
    double AtomicForceConstant;
    double AtomicRadius;
    double Dissipation;
    int ParticleCount;
    double DefaultParticleRadius;
    double DefaultParticleMass;
    double DefaultParticleCharge;
    int Scale;

   bool CheckCollisions;
   int ResolveCollisionsRetries;

    Vector BorderDimensions;
    bool DoInteraction;

    RGBData UnchargedParticleColor;
    RGBData PositivelyChargedParticleColor;
    RGBData NegativeChargedParticleColor;
    RGBData BackgroundColor;

    bool DrawVelocities;
    bool DrawVelocitiesDoNotRedrawPrevious;
    double VelocityLengthFactor;
    RGBData VelocityColor;

    int MaxFPS;
    double MaxAllowedForce;
    bool RestrictInterParticleForce;
    double MaxInterParticleForce;
    bool WriteInterParticleForceWarning;
    bool ResetOnMaxAllowedForce;
};

#endif // PARTICLES_PARAMETERS_H
