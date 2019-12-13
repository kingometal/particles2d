#include "ParticleManager.h"
#include "Particle.h"

using namespace std;

#define MAX_FLOAT 3.40282347e+38F

class ParticleManagerImpl
{
public:
    ParticleManagerImpl(int particleCount)
        : ParticleCount(particleCount)
    {
        ParticleN = new Particle[ParticleCount];
        Forces = new Vector[ParticleCount*ParticleCount]();
        SkippedForceCalculations = new int[ParticleCount*ParticleCount]();
    }

    ~ParticleManagerImpl()
    {
        delete [] ParticleN;
    }

    int AddParticle(const Vector& position, const Vector& velocity, const double mass, const double charge, const double radius)
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
        ParticleN[ParticleCount].Radius = radius;

        delete (Forces);
        delete (SkippedForceCalculations);

        ParticleCount++;

        Forces = new Vector[ParticleCount*ParticleCount]();
        SkippedForceCalculations = new int[ParticleCount*ParticleCount]();
        for (int i = 0; i < ParticleCount*ParticleCount; ++i)
        {
            Forces[i].Reset();
            SkippedForceCalculations[i] = 0;
        }

        return ParticleCount;
    }

    void RemoveParticle(const int index)
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

        delete (Forces);
        delete (SkippedForceCalculations);

        ParticleCount--;

        Forces = new Vector[ParticleCount*ParticleCount]();
        SkippedForceCalculations = new int[ParticleCount*ParticleCount]();

        for (int i = 0; i < ParticleCount*ParticleCount; ++i)
        {
            Forces[i].Reset();
            SkippedForceCalculations[i] = 0;
        }
    }

    void UpdatePosition(int index)
    {
        ParticleN[index].Position += ParticleN[index].Velocity;
    }

    void ChangeRadius(int index, double delta)
    {
        if (delta < 0)
        {
            if (ParticleN[index].Radius > - delta) {
                ParticleN[index].Radius += delta;
            }
        }
        else
        {
            ParticleN[index].Radius += delta;
        }
    }

    inline double OneDcollision(double v, double v2, double m, double m2) const
    {   // returns final velocity of the particle 1 after collision with particle 2
        return (2.0 * m2 * v2   + (m - m2)* v) / (m + m2);
    }

    void PerformCollision (int index1, int index2)
    {
        Particle& p1 = ParticleN[index1];
        Particle& p2 = ParticleN[index2];
        Vector distance = p1.Position - p2.Position;
        double tanr = (distance.Get(1) / distance.Get(0));
        double cosr = 1/sqrt(tanr*tanr + 1.0);
        double sinr = tanr*cosr;

        if (p1.Position.Get(0) - p2.Position.Get(0) == 0.0)
        {
            cosr = 0.0;
            sinr = 1.0;
        }

        // rotate both particle positions so that their connection is parallel to the X axis
        Vector vRotated1 (p1.Velocity * cosr + Vector (sinr * p1.Velocity.Y(), - sinr * p1.Velocity.X()));
        Vector vRotated2 (p2.Velocity * cosr + Vector (sinr * p2.Velocity.Y(), - sinr * p2.Velocity.X()));

        // perform collision along the X axis
        double vRotatedTempX = vRotated1.X();
        vRotated1.v[0] = OneDcollision (vRotated1.X(), vRotated2.X(), p1.Mass, p2.Mass);
        vRotated2.v[0] = OneDcollision (vRotated2.X(), vRotatedTempX, p2.Mass, p1.Mass);

        // rotate both particle positions back
        p1.Velocity = vRotated1 * cosr + Vector (- sinr * vRotated1.Y(), sinr * vRotated1.X());
        p2.Velocity = vRotated2 * cosr + Vector (- sinr * vRotated2.Y(), sinr * vRotated2.X());
    }

    Vector GetForce(int index1, int index2)
    {
        return Forces[index1*ParticleCount + index2];
    }

    void SetForce(int index1, int index2, Vector v)
    {
        Forces[index1*ParticleCount + index2] = v;
    }

    Vector GetFullForce(int index)
    {
        Vector sum;
        int startposition = index * ParticleCount;
        Vector* currentForce = &Forces[startposition];
        for (int i = 0; i < ParticleCount; ++i)
        {
            sum += *currentForce;
            currentForce++;
        }
        return sum;
    }

    int ParticleCount;
    Vector *Forces;
    Particle* ParticleN;
    int* SkippedForceCalculations;
};

ParticleManager::ParticleManager(int particleCount)
    : Impl(new ParticleManagerImpl(particleCount))
{
}

ParticleManager::~ParticleManager()
{
    delete Impl;
}

int ParticleManager::GetClosestParticle(int x, int y) const
{
    int closestParticle = -1;
    double shortestDistance = MAX_FLOAT;
    Vector position(x,y);
    for (int i = Impl->ParticleCount -  1; i >= 0; --i)
    {
        double distance = (position - Impl->ParticleN[i].Position).Abs();
        if (distance < shortestDistance)
        {
            closestParticle = i;
            shortestDistance = distance;
        }
    }
    return closestParticle;
}

int ParticleManager::AddParticle(const Vector& position, const Vector& velocity, const double mass, const double charge, const double radius)
{
    Impl->AddParticle(position, velocity, mass, charge, radius);
}

void ParticleManager::RemoveParticle(const int index)
{
    Impl->RemoveParticle(index);
}

void ParticleManager::UpdateVelocity(const int index, const Vector &maxCoord, const double dissipation)
{
    double acceleration;
    double nextPossiblePosition;
    Particle & p = Impl->ParticleN[index];
    Vector force = Impl->GetFullForce(index);
    for (int n = 0 ; n < 2 ; n++)
    {
        acceleration = force.Get(n) / p.Mass;
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

void ParticleManager::UpdatePosition(const int index)
{
    Impl->UpdatePosition(index);
}

void ParticleManager::ChangeRadius(const int index, const double delta)
{
    Impl->ChangeRadius(index, delta);
}

void ParticleManager::SetPosition(const int index, const Vector &newPosition)
{
    Impl->ParticleN[index].Position = newPosition;
}

int ParticleManager::PCount() const
{
    return Impl->ParticleCount;
}

const Particle &ParticleManager::P(int index) const
{
    return Impl->ParticleN[index];
}

double ParticleManager::Distance(int index1, int index2) const
{
    return (Impl->ParticleN[index2].Position - Impl->ParticleN[index1].Position).Abs();
}

void ParticleManager::PerformCollision (int index1, int index2) {

    Impl->PerformCollision (index1, index2);
}

Vector ParticleManager::GetFullForce(int index) const
{
    return Impl->GetFullForce(index);
}

void ParticleManager::StoreForce(int index1, int index2, Vector force)
{
    Impl->SetForce(index1, index2, force);
    Impl->SetForce(index2, index1, -force);
}

void ParticleManager::SkipForceCalculation(int index1, int index2)
{
    Impl->SkippedForceCalculations[index1 * Impl->ParticleCount + index2]++;
}

int ParticleManager::GetNumSkippedForceCalculations(int index1, int index2)
{
    return Impl->SkippedForceCalculations[index1 * Impl->ParticleCount + index2];
}

void ParticleManager::ResetNumSkippedForceCalculations(int index1, int index2)
{
    Impl->SkippedForceCalculations[index1 * Impl->ParticleCount + index2] = 0;
}
