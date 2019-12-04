#include "Config.h"
#include <libconfig.h>
#include <iostream>

namespace
{
    void GetConfigDouble(const config_t& config, const char* key, double& target)
    {
        if (!config_lookup_float(&config, key, &target))
        {
            std::cout << "failed to get key " << key << "; using default value: " << target << std::endl;
        }
        else
        {
            std::cout << "loaded from config: " << key << " : " << target << std::endl;
        }
    }

    void GetConfigInt(const config_t& config, const char* key, int& target)
    {
        if (!config_lookup_int(&config, key, &target))
        {
            std::cout << "failed to get key " << key << "; using default value: " << target  << std::endl;
        }
        else
        {
            std::cout << "loaded from config: " << key << " : " << target  << std::endl;
        }
    }

    void GetConfigBool(const config_t& config, const char* key, bool& target)
    {
        int temp;
        if (!config_lookup_bool(&config, key, &temp))
        {
            std::cout << "failed to get key " << key << "; using default value: " << target  << std::endl;
            target = temp;
        }
        else
        {
            target = temp;
            std::cout << "loaded from config: " << key << " : " << target  << std::endl;
        }
    }
}

Config::Config(void)
    : WindowSideSizeX(800)
    , WindowSideSizeY(600)
    , GravitationalConstant (204.8)
    , ElectrostaticConstant (409.6)
    , MagneticPermeability (0.1)
    , MolecularBondingEnergy(0.001)
    , AtomicRadius(20.0)
    , Dissipation(0.000001)
    , ParticleCount(200)
    , DefaultParticleRadius(10)
    , DefaultParticleMass(1.0)
    , DefaultParticleCharge(1.0)
    , Scale(2.0)
    , BorderDimensions()
    , DoInteraction(true)
    , CheckCollisions (true)
    , ResolveCollisionsRetries(3)
    , UnchargedParticleColor(0, 0, 0, 255)
    , PositivelyChargedParticleColor(255, 0, 0, 255)
    , NegativeChargedParticleColor(0, 0, 255, 255)
    , BackgroundColor(255, 255, 255, 255)
    , MaxFPS(100)
    , MaxAllowedForce(100.0)
    , RestrictInterParticleForce(false)
    , MaxInterParticleForce(1.0)
    , WriteInterParticleForceWarning(true)
    , ResetOnMaxAllowedForce(true)
{
    config_t config;
    int tempColorR;
    int tempColorG;
    int tempColorB;

    config_init(&config);

    if (!config_read_file(&config, "Particles.cfg"))
    {
        std::cout << "Can not get parameter file" << std::endl;
        config_destroy(&config);
        return;
    }

    GetConfigInt(config, "WindowSideSizeX", WindowSideSizeX);
    GetConfigInt(config, "WindowSideSizeY", WindowSideSizeY);
    GetConfigDouble(config, "GravitationalConstant", GravitationalConstant);
    GetConfigDouble(config, "ElectrostaticConstant", ElectrostaticConstant);
    GetConfigDouble(config, "MagneticPermeability", MagneticPermeability);
    GetConfigDouble(config, "MolecularBondingEnergy", MolecularBondingEnergy);
    GetConfigDouble(config, "AtomicRadius", AtomicRadius);
    GetConfigDouble(config, "Dissipation", Dissipation);
    GetConfigInt(config, "ParticleCount", ParticleCount);
    GetConfigDouble(config, "DefaultParticleRadius", DefaultParticleRadius);
    GetConfigDouble(config, "DefaultParticleMass", DefaultParticleMass);
    GetConfigDouble(config, "DefaultParticleCharge", DefaultParticleCharge);
    GetConfigInt(config, "Scale", Scale);
    GetConfigBool(config, "DoInteraction", DoInteraction);
    GetConfigBool(config, "CheckCollisions", CheckCollisions);
    GetConfigInt(config, "ResolveCollisionsRetries", ResolveCollisionsRetries);



    GetConfigInt(config, "UnchargedParticleColorR", tempColorR);
    GetConfigInt(config, "UnchargedParticleColorG", tempColorG);
    GetConfigInt(config, "UnchargedParticleColorB", tempColorB);
    UnchargedParticleColor = RGBData(tempColorR, tempColorG, tempColorB, 255);

    GetConfigInt(config, "PositivelyChargedParticleColorR", tempColorR);
    GetConfigInt(config, "PositivelyChargedParticleColorG", tempColorG);
    GetConfigInt(config, "PositivelyChargedParticleColorB", tempColorB);
    PositivelyChargedParticleColor = RGBData(tempColorR, tempColorG, tempColorB, 255);

    GetConfigInt(config, "NegativeChargedParticleColorR", tempColorR);
    GetConfigInt(config, "NegativeChargedParticleColorG", tempColorG);
    GetConfigInt(config, "NegativeChargedParticleColorB", tempColorB);
    NegativeChargedParticleColor = RGBData(tempColorR, tempColorG, tempColorB, 255);

    GetConfigInt(config, "BackgroundColorR", tempColorR);
    GetConfigInt(config, "BackgroundColorG", tempColorG);
    GetConfigInt(config, "BackgroundColorB", tempColorB);
    BackgroundColor = RGBData(tempColorR, tempColorG, tempColorB, 255);

    GetConfigInt(config, "MaxFPS", MaxFPS);

    GetConfigBool(config, "ResetOnMaxAllowedForce", ResetOnMaxAllowedForce);
    GetConfigDouble(config, "MaxAllowedForce", MaxAllowedForce);

    GetConfigBool(config, "RestrictInterParticleForce", RestrictInterParticleForce);
    GetConfigDouble(config, "MaxInterParticleForce", MaxInterParticleForce);
    GetConfigBool(config, "WriteInterParticleForceWarning", WriteInterParticleForceWarning);

    config_destroy(&config);
}
