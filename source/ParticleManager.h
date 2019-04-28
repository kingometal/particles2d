#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

class ParticleManagerImpl;

class ParticleManager
{
public:
    ParticleManager(int particleCount);
    ~ParticleManager();
    void InitParticle(int index, double* position, double* velocity, double mass, double charge, double radius);
    void AddParticle(double* position, double* velocity, double mass, double charge, double radius);
    void RemoveParticle(int index);
    void UpdateVelocity(int index, double *maxCoord, double dissipation);
    void UpdatePosition(int index);
    void ResetForce(int index);
    void ResetForces();
    void ChangeRadius(int index, double delta);

    int PCount();

    double Velocity(int index, int dim);
    double Position(int index, int dim);
    double Force(int index, int dim);
    double Distance(int index1, int index2);
    double Radius(int index);
    double Mass(int index);
    double Charge(int index);
    bool CheckCollisionImminent(int index1, int index2);
    bool CheckOverlap(int index1, int index2, double distance);
    void ResolveOverlapIfNeeded(int index1, int index2, double distance);
    void PerformCollision(int index1, int index2);

    void AddForce(int index, double dfx, double dfy);
private:
    ParticleManagerImpl* Impl;
    inline double OneDcollision(double v, double v2, double m, double m2);
};

#endif // PARTICLEMANAGER_H
