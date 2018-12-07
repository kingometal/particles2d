#ifndef PARTICLES_H
#define PARTICLES_H

#include <ctime>


class Particle;
class ParticlesViewInterface;
class IUserInput;

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
    void sleeps(clock_t wait);

private:
    void redrawV(int color);
    void redrawV();
    void RedrawParticleAtNewPosition(int index, double oldX, double oldY, double x, double y, double q);
    void RedrawParticleAtNewPosition(int index);
    void ApplyForce(int n1, int n2);
    void collision(int n1, int n2);
    void ResolveOverlapIfNeeded(int n1, int n2, double r);
    bool CheckCollisionImminent(int n1, int n2);
    bool CheckOverlap(int n1, int n2, double distance);
    double getR(int n1, int n2);
    double oneDcollision(double v, double v2, double m, double m2);

    const int L;
    ParticlesViewInterface &W;
    IUserInput& UserInput;

    const int NumParticles;
    Particle* ParticleN;
    double GravitationalConstant;
    double ElectrostaticConstant;
    const double MagneticPermeability;
    double MolecularBondingEnergy;
    void HandleKeyPress();
    double Mass(int n);
    double Charge(int n);
    void AvoidCollisions();
    void UpdateParticlesPositionsAndDraw();
    void ResetParticlesForces();
    void UpdateParticlesForcesAndVelocities();
};

#endif


