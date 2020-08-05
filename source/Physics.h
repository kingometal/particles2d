#ifndef PHYSICS_H
#define PHYSICS_H

class Vector;
class ParticleManager;
class Config;
class Particle;
class PhysicalConstants;

namespace Physics
{
Vector GetGravity(const Particle& p1, const Particle& p2, double reziprocalDistance, double G);

Vector GetElectromagneticForce(const Particle& p1, const Particle& p2, double reziprocalDistance, double E, double M);

Vector GetLennardJonesForce(const Particle& p1, const Particle& p2, double distance, double reziprocalDistance, double atomicRadius, double bondingEnergy);

Vector GetInterParticleForce(const Particle& p1, const Particle& p2, const PhysicalConstants& Params);

double eKin(const ParticleManager* PManager);

double eEl(const ParticleManager* PManager, const Config & Params);

double eG(const ParticleManager* PManager, const Config & Params);

double eMol(const ParticleManager* PManager, const Config & Params);
} // namespace Physics

#endif // PHYSICS_H
