#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

class ParticleManagerImpl;
class Vector;
class Particle;

class ParticleManager
{
public:
    ParticleManager(int particleCount);
    ~ParticleManager();
    int AddParticle(const Vector &position, const Vector &velocity, const double mass, const double charge, const double radius);
    void RemoveParticle(const int index);
    void UpdateVelocity(const int index, const Vector &maxCoord, const double dissipation);
    void UpdatePosition(const int index);
    void ResetForce(const int index);
    void ResetForces();
    void AddForce(int index, Vector force);
    void StoreForce(int index1, int index2, Vector force);
    void SkipForceCalculation(int index1, int index2);
    int GetNumSkippedForceCalculations(int index1, int index2);
    void ResetNumSkippedForceCalculations(int index1, int index2);

    void ChangeRadius(const int index, const double delta);
    void SetPosition(const int index, const Vector& newPosition);

    int PCount() const;

    double Distance(int index1, int index2) const;
    void PerformCollision(int index1, int index2);
    void FuseParticles(int index1, int index2);
    Vector GetFullForce(int index) const;

    const Particle& P(int index);
private:
    ParticleManagerImpl* Impl;
};

#endif // PARTICLEMANAGER_H
