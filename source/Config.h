#ifndef PARTICLES_PARAMETERS_H
#define PARTICLES_PARAMETERS_H

#include "RGBData.h"
#include "Vector.h"

struct PhysicalConstants
{
    PhysicalConstants()
        : GravitationalConstant (204.8)
        , ElectrostaticConstant (409.6)
        , MagneticPermeability (0.1)
        , MolecularBondingEnergy(0.001)
        , AtomicRadius(20.0)
    {

    }

    double GravitationalConstant;
    double ElectrostaticConstant;
    double MagneticPermeability;
    double MolecularBondingEnergy;
    double AtomicForceConstant;
    double AtomicRadius;
};

class Config
{
public:
    Config(void);

    int WindowSideSizeX;
    int WindowSideSizeY;
    PhysicalConstants PhysConstants;
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
