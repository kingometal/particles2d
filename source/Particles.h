#ifndef PARTICLES_H
#define PARTICLES_H

#include <ctime>

class Particle;
class ParticlesViewInterface;
class IUserInput;
class ParticleManager;
class Config;
class Vector;
class RGBData;

class Particles{
public:
    Particles(ParticlesViewInterface& window, IUserInput& userInput, Config& parameters);
    ~Particles();
    void Update();
    void Init();

    double eEl();
    double eKin();
    double eMol();
    double eG();
    void Sleep(clock_t wait);

private:
    void DrawVelocities(RGBData* color) const;
    void RedrawParticleAtNewPosition(int index, const Vector &oldPosition, const Vector &newPosition, double q);
    void ApplyForce(int n1, int n2);

    ParticlesViewInterface &W;
    IUserInput& UserInput;
    ParticleManager* PManager;
    Config& Params;

    void HandleKeyPress();
    void AvoidCollisions();
    void UpdateParticlesPositionsAndDraw();
    void UpdateParticlesForcesAndVelocities();
    void AddParticle(int x, int y, double dx, double dy);
    bool RemoveParticle(int x, int y);
    void UpdateParticleSpeed(int n);
    bool CheckCollisionImminent(const int index1, const int index2) const;
    void ReInit();
    void ResolveOverlapIfNeeded(int index1, int index2, double distance);
    bool CheckOverlap(const int index1, const int index2, const double distance) const;
    void VelocityDrawRemoveOld();
    void VelocityDrawNew();
};

#endif


