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
    void InitParticle(int index, Vector &position, Vector &velocity, double mass, double charge, double radius);
    void AddParticle(Vector &position, Vector &velocity, double mass, double charge, double radius);
    void RemoveParticle(int index);
    void UpdateVelocity(const int index, const Vector &maxCoord, const double dissipation);
    void UpdatePosition(int index);
    void ResetForce(int index);
    void ResetForces();
    void ChangeRadius(int index, double delta);

    int PCount();

//    double Velocity(int index, int dim);
//    double Position(int index, int dim);
//    double Force(int index, int dim);
    double Distance(int index1, int index2);
//    double Radius(int index);
//    double Mass(int index);
//    double Charge(int index);
    bool CheckCollisionImminent(int index1, int index2);
    void ResolveOverlapIfNeeded(int index1, int index2, double distance);
    void PerformCollision(int index1, int index2);

    void AddForce(int index, double dfx, double dfy);
    const Particle& P(int index);
private:
    ParticleManagerImpl* Impl;
};

#endif // PARTICLEMANAGER_H
