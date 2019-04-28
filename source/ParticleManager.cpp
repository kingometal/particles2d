#include "ParticleManager.h"
#include "Particle.h"

#include <cmath>

class ParticleManagerImpl
{
public:
    ParticleManagerImpl(int particleCount)
        : ParticleCount(particleCount)
    {
        ParticleN = new Particle[particleCount];
    }

    ~ParticleManagerImpl()
    {

    }

    void InitParticle(int index, Vector& position, Vector& velocity, double mass, double charge, double radius)
    {
        ParticleN[index].Position = position;
        ParticleN[index].Mass = mass;
        ParticleN[index].Charge = charge;
        ParticleN[index].Velocity = velocity;
        ParticleN[index].Force.Set(0.0, 0.0);
        ParticleN[index].Radius = radius;
    }

    void AddParticle(Vector& position, Vector& velocity, double mass, double charge, double radius)
    {
        Particle* tempParticlesN = new Particle[ParticleCount + 1];
        for (int i = 0; i < ParticleCount; i++)
        {
            tempParticlesN[i] = ParticleN[i];
        }
        delete [] ParticleN;
        ParticleN = tempParticlesN;

        ParticleN[ParticleCount].Position = position;
        ParticleN[ParticleCount].Mass = mass;
        ParticleN[ParticleCount].Charge = charge;
        ParticleN[ParticleCount].Velocity = velocity;
        ParticleN[ParticleCount].Force.Set(0.0, 0.0);
        ParticleN[ParticleCount].Radius = radius;

        ParticleCount++;
    }

    void RemoveParticle(int index)
    {
        Particle* tempParticlesN = new Particle[ParticleCount - 1];
        int j = 0;
        for (int i = 0; i < ParticleCount - 1; i++)
        {
            if (i == index)
            {
                j++;
            }
            tempParticlesN[i] = ParticleN[j];
            j++;

        }
        delete [] ParticleN;
        ParticleN = tempParticlesN;
        ParticleCount--;
    }

    void UpdateVelocity(const int index, const Vector &maxCoord, const double dissipation)
    {
        double acceleration;
        double nextPossiblePosition;
        Particle & p = ParticleN[index];

        for (int n = 0 ; n < 2 ;n++)
        {
            acceleration = p.Force.Get(n) * p.ReciMass;
            nextPossiblePosition = p.Position.Get(n) + p.Velocity.Get(n) + acceleration;
            if (nextPossiblePosition >= maxCoord.Get(n) || nextPossiblePosition < 0)
            {
                p.Velocity.v[n] = -p.Velocity.v[n];
            }
            else
            {
                p.Velocity.v[n] += acceleration;
            }
        }

        p.Velocity = p.Velocity * (1 - dissipation);
    }

    void UpdatePosition(int index)
    {
        ParticleN[index].Position += ParticleN[index].Velocity;
    }

    void ResetForces()
    {
        for (int i = 0; i < ParticleCount; i++)
        {
            ParticleN[i].Force.Reset();
        }
    }

    void ChangeRadius(int index, double delta)
    {
        if (delta < 0)
        {
            if (ParticleN[index].Radius > - delta){
                ParticleN[index].Radius += delta;
            }
        }
        else
        {
            ParticleN[index].Radius += delta;
        }
    }

    double Distance(int index1, int index2)
    {
        return (ParticleN[index2].Position - ParticleN[index1].Position).Abs();
    }

    bool CheckCollisionImminent(int index1, int index2)
    {
    //    return Impl->ParticleN[index1].CheckCollisionImminent(Impl->ParticleN[index2]);
        return ((ParticleN[index2].Position + ParticleN[index2].Velocity) - (ParticleN[index1].Position + ParticleN[index1].Velocity)).Abs() < ParticleN[index2].Radius + ParticleN[index1].Radius;
    }

    bool CheckOverlap(int index1, int index2, double distance)
    {
        return distance < ParticleN[index1].Radius + ParticleN[index2].Radius;
    }

    void ResolveOverlapIfNeeded(int index1, int index2, double distance)
    {
        if (CheckOverlap(index1, index2, distance)){
            double x1 = ParticleN[index1].Position.v[0];
            double x2 = ParticleN[index2].Position.v[0];
            double y1 = ParticleN[index1].Position.v[1];
            double y2 = ParticleN[index2].Position.v[1];

            double rx = x2 - x1;
            double ry = y2 - y1;
            double cm = (ParticleN[index2].Mass*distance)/(ParticleN[index2].Mass + ParticleN[index1].Mass);  //center of mass
            double cmx = x1 + rx*cm/distance;
            double cmy = y1 + ry*cm/distance;

            double R = ParticleN[index1].Radius + ParticleN[index2].Radius;
            double quot = R/distance;
            ParticleN[index2].Position.Set((x2-cmx)*quot + cmx, (y2-cmy)*quot + cmy);
            ParticleN[index1].Position.Set((x1-cmx)*quot + cmx, (y1-cmy)*quot + cmy);
        }
    }

    inline double OneDcollision(double v, double v2, double m, double m2){
        return (2.0 * m2 * v2   + (m - m2)* v) / (m + m2);
    }

    void PerformCollision (int index1, int index2){
        Vector distance = ParticleN[index1].Position - ParticleN[index2].Position;
        double tanr = (distance.Get(1) / distance.Get(0));
        double cosr = 1/sqrt(tanr*tanr + 1.0);
        double sinr = tanr*cosr;

        if (ParticleN[index1].Position.Get(0) - ParticleN[index2].Position.Get(0) == 0.0) {
            tanr = 0.0;
            cosr = 0.0;
            sinr = 1.0;
        }

        double vRotatedx1 = cosr * ParticleN[index1].Velocity.Get(0) + sinr * ParticleN[index1].Velocity.Get(1);
        double vRotatedy1 = cosr * ParticleN[index1].Velocity.Get(1) - sinr * ParticleN[index1].Velocity.Get(0);
        double vRotatedx2 = cosr * ParticleN[index2].Velocity.Get(0) + sinr * ParticleN[index2].Velocity.Get(1);
        double vRotatedy2 = cosr * ParticleN[index2].Velocity.Get(1) - sinr * ParticleN[index2].Velocity.Get(0);

        double vGedrehttemp = vRotatedx1;
        vRotatedx1 = OneDcollision (vRotatedx1, vRotatedx2, ParticleN[index1].Mass, ParticleN[index2].Mass);
        vRotatedx2 = OneDcollision (vRotatedx2, vGedrehttemp, ParticleN[index2].Mass, ParticleN[index1].Mass);

        vGedrehttemp = vRotatedx1;
        vRotatedx1 = cosr * vRotatedx1 - sinr * vRotatedy1;
        vRotatedy1 = cosr * vRotatedy1 + sinr * vGedrehttemp;
        vGedrehttemp = vRotatedx2;
        vRotatedx2 = cosr * vRotatedx2 - sinr * vRotatedy2;
        vRotatedy2 = cosr * vRotatedy2 + sinr * vGedrehttemp;

        ParticleN[index1].Velocity.Set(vRotatedx1, vRotatedy1);
        ParticleN[index2].Velocity.Set(vRotatedx2, vRotatedy2);
    }

    void AddForce(int index, double dfx, double dfy)
    {
        Vector df (dfx, dfy);
        ParticleN[index].Force += df;
    }

    int ParticleCount;
    Particle* ParticleN;
};



ParticleManager::ParticleManager(int particleCount)
    : Impl(new ParticleManagerImpl(particleCount))
{

}

ParticleManager::~ParticleManager()
{
    delete Impl;
}

void ParticleManager::InitParticle(int index, Vector& position, Vector& velocity, double mass, double charge, double radius)
{
    Impl->InitParticle(index, position, velocity, mass, charge, radius);
}

void ParticleManager::AddParticle(Vector& position, Vector& velocity, double mass, double charge, double radius)
{
    Impl->AddParticle(position, velocity, mass, charge, radius);
}

void ParticleManager::RemoveParticle(int index)
{
    Impl->RemoveParticle(index);
}

void ParticleManager::UpdateVelocity(const int index, const Vector &maxCoord, const double dissipation)
{
    Impl->UpdateVelocity(index, maxCoord, dissipation);
}

void ParticleManager::UpdatePosition(int index)
{
    Impl->UpdatePosition(index);
}

void ParticleManager::ResetForce(int index)
{
    Impl->ParticleN[index].Force.Reset();
}

void ParticleManager::ResetForces()
{
    Impl->ResetForces();
}

void ParticleManager::ChangeRadius(int index, double delta)
{
    Impl->ChangeRadius(index, delta);
}

int ParticleManager::PCount()
{
    return Impl->ParticleCount;
}

const Particle &ParticleManager::P(int index)
{
    return Impl->ParticleN[index];
}

double ParticleManager::Distance(int index1, int index2)
{
    return (Impl->ParticleN[index2].Position - Impl->ParticleN[index1].Position).Abs();
}

bool ParticleManager::CheckCollisionImminent(int index1, int index2)
{
    return Impl->CheckCollisionImminent(index1, index2);
}

void ParticleManager::ResolveOverlapIfNeeded(int index1, int index2, double distance)
{
    Impl->ResolveOverlapIfNeeded(index1, index2, distance);
}

void ParticleManager::PerformCollision (int index1, int index2){

    Impl->PerformCollision (index1, index2);
}

void ParticleManager::AddForce(int index, double dfx, double dfy)
{
    Impl->AddForce(index, dfx, dfy);
}
