#ifndef PARTICLES_H
#define PARTICLES_H

#include <ctime>
#include "Vector.h"


class Particle;
class ParticlesViewInterface;
class IUserInput;
class ParticleManager;

class Particles{
public:
    Particles(ParticlesViewInterface& window, IUserInput& userInput);
    ~Particles();
    void Update();
    void Init();

    double eEl();
    double eKin();
    double eMol();
    double eG();
    void Sleep(clock_t wait);

private:
    void RedrawV(int color);
    void RedrawV();
    void RedrawParticleAtNewPosition(int index, const Vector &oldPosition, const Vector &newPosition, double q);
    Vector ApplyForce(int n1, int n2);

    ParticlesViewInterface &W;
    IUserInput& UserInput;
    ParticleManager* PManager;

    double GravitationalConstant;
    double ElectrostaticConstant;
    const double MagneticPermeability;
    double MolecularBondingEnergy;
    Vector BorderDimensions;
    bool DoInteraction;

    void HandleKeyPress();
    void AvoidCollisions();
    void UpdateParticlesPositionsAndDraw();
    void UpdateParticlesForcesAndVelocities();
    void AddParticle(int x, int y, double dx, double dy);
    bool RemoveParticle(int x, int y);
    int GetClosestParticle(int x, int y);
    void UpdateParticleSpeed(int n);
    bool CheckCollisionImminent(const int index1, const int index2) const;
    void ReInit();
    void ResolveOverlapIfNeeded(int index1, int index2, double distance);
    bool CheckOverlap(const int index1, const int index2, const double distance) const;
};

#endif


