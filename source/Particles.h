#ifndef PARTICLES_H
#define PARTICLES_H

#include <ctime>

class IParticlesView;
class IUserInput;
class ParticleManager;
class Config;
class ParticleDrawer;

class Particles {
public:
    Particles(IParticlesView &window, IUserInput& userInput, Config& parameters);
    ~Particles();
    void Update();
    void Init();

    void Sleep(clock_t wait);

private:
    void ApplyForce(int n1, int n2);

    IParticlesView &W;
    IUserInput& UserInput;
    ParticleManager* PManager;
    Config& Params;
    ParticleDrawer* Drawer;

    void HandleKeyPress();
    void AvoidCollisions();
    void UpdateParticlesPositions();
    void UpdateParticlesForcesAndVelocities();
    void AddParticle(int x, int y, double dx, double dy);
    bool RemoveParticle(int x, int y);
    void UpdateParticleSpeed(int n);
    bool CheckCollisionImminent(const int index1, const int index2) const;
    void ReInit();
    void ResolveOverlapIfNeeded(int index1, int index2, double distance);
    bool CheckOverlap(const int index1, const int index2, const double distance) const;
    void CheckParticlePositionsAreWithinWindow();
    void UpdateBorderDimensions();
    void Draw() const;
};

#endif


