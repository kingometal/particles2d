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

void ParticleManager::InitParticle(int index, double *position, double *velocity, double mass, double charge, double radius)
{
    Impl->ParticleN[index].SetPosition(position[0], position[1]);
    Impl->ParticleN[index].SetMass(mass);
    Impl->ParticleN[index].SetCharge(charge);
    Impl->ParticleN[index].SetVelocity(velocity[0],velocity[1]);
    Impl->ParticleN[index].SetForce(0.0, 0.0);
    Impl->ParticleN[index].SetRadius(radius);
}

void ParticleManager::AddParticle(double *position, double *velocity, double mass, double charge, double radius)
{
    Particle* tempParticlesN = new Particle[Impl->ParticleCount + 1];
    for (int i = 0; i < Impl->ParticleCount; i++)
    {
        tempParticlesN[i] = Impl->ParticleN[i];
    }
    delete [] Impl->ParticleN;
    Impl->ParticleN = tempParticlesN;

    Impl->ParticleN[Impl->ParticleCount].SetPosition(position[0], position[1]);
    Impl->ParticleN[Impl->ParticleCount].SetMass(mass);
    Impl->ParticleN[Impl->ParticleCount].SetCharge(charge);
    Impl->ParticleN[Impl->ParticleCount].SetVelocity(velocity[0],velocity[1]);
    Impl->ParticleN[Impl->ParticleCount].SetForce(0.0, 0.0);
    Impl->ParticleN[Impl->ParticleCount].SetRadius(radius);

    Impl->ParticleCount++;
}

void ParticleManager::RemoveParticle(int index)
{
    Particle* tempParticlesN = new Particle[Impl->ParticleCount - 1];
    int j = 0;
    for (int i = 0; i < Impl->ParticleCount - 1; i++)
    {
        if (i == index)
        {
            j++;
        }
        tempParticlesN[i] = Impl->ParticleN[j];
        j++;

    }
    delete [] Impl->ParticleN;
    Impl->ParticleN = tempParticlesN;
    Impl->ParticleCount--;
}

void ParticleManager::UpdateVelocity(int index, double *maxCoord, double dissipation)
{
    Impl->ParticleN[index].UpdateSpeedAccordingToForceDissipationAndBorders(maxCoord, dissipation);
}

void ParticleManager::UpdatePosition(int index)
{
    Impl->ParticleN[index].MoveAccordingToCurrentVelocity();
}

void ParticleManager::ResetForce(int index)
{
    Impl->ParticleN[index].SetForce(0.0, 0.0);
}

void ParticleManager::ResetForces()
{
    for (int i = 0; i < Impl->ParticleCount; i++)
    {
        Impl->ParticleN[i].SetForce(0.0, 0.0);
    }
}

void ParticleManager::ChangeRadius(int index, double delta)
{
    if (delta < 0)
    {
        if (Impl->ParticleN[index].GetRadius() > - delta){
            Impl->ParticleN[index].SetRadius(Impl->ParticleN[index].GetRadius() + delta);
        }
    }
    else
    {
        Impl->ParticleN[index].SetRadius(Impl->ParticleN[index].GetRadius() + delta);
    }
}

int ParticleManager::PCount()
{
    return Impl->ParticleCount;
}


double ParticleManager::Velocity(int index, int dim)
{
    return Impl->ParticleN[index].GetVelocity(dim);
}

double ParticleManager::Position(int index, int dim)
{
    return Impl->ParticleN[index].GetPosition(dim);
}

double ParticleManager::Force(int index, int dim)
{
    return Impl->ParticleN[index].GetForce(dim);
}

double ParticleManager::Distance(int index1, int index2)
{
    return Impl->ParticleN[index1].GetDistance(Impl->ParticleN[index2]);
}

double ParticleManager::Radius(int index)
{
    return Impl->ParticleN[index].GetRadius();
}

double ParticleManager::Mass(int index)
{
    return Impl->ParticleN[index].GetMass();
}

double ParticleManager::Charge(int index)
{
    return Impl->ParticleN[index].GetCharge();
}

bool ParticleManager::CheckCollisionImminent(int index1, int index2)
{
    return Impl->ParticleN[index1].CheckCollisionImminent(Impl->ParticleN[index2]);
}

bool ParticleManager::CheckOverlap(int index1, int index2, double distance)
{
    return distance < Radius(index1) + Radius(index2);
}

void ParticleManager::ResolveOverlapIfNeeded(int index1, int index2, double distance)
{
    if (CheckOverlap(index1, index2, distance)){
        double x1 = Position(index1, 0);
        double x2 = Position(index2, 0);
        double y1 = Position(index1, 1);
        double y2 = Position(index2, 1);

        double rx = x2 - x1;
        double ry = y2 - y1;
        double cm = (Mass(index2)*distance)/(Mass(index2)+Mass(index1));  //center of mass
        double cmx = x1 + rx*cm/distance;
        double cmy = y1 + ry*cm/distance;

        double R = Radius(index1) + Radius(index2);
        double quot = R/distance;
        Impl->ParticleN[index2].SetPosition((x2-cmx)*quot + cmx, (y2-cmy)*quot + cmy);
        Impl->ParticleN[index1].SetPosition((x1-cmx)*quot + cmx, (y1-cmy)*quot + cmy);
    }
}

double ParticleManager::OneDcollision(double v, double v2, double m, double m2){
    return (2.0 * m2 * v2   + (m - m2)* v) / (m + m2);
}

void ParticleManager::PerformCollision (int index1, int index2){

    double tanr = (Impl->ParticleN[index1].GetYPosition()-Impl->ParticleN[index2].GetYPosition())/(Impl->ParticleN[index1].GetXPosition() - Impl->ParticleN[index2].GetXPosition());
    double cosr = 1/sqrt(tanr*tanr + 1.0);
    double sinr = tanr*cosr;

    if (Impl->ParticleN[index1].GetXPosition() - Impl->ParticleN[index2].GetXPosition() == 0.0) {
        tanr = 0.0;
        cosr = 0.0;
        sinr = 1.0;
    }

    double vRotatedx1 = cosr*Impl->ParticleN[index1].GetXVelocity() + sinr*Impl->ParticleN[index1].GetYVelocity();
    double vRotatedy1 = cosr*Impl->ParticleN[index1].GetYVelocity() - sinr*Impl->ParticleN[index1].GetXVelocity();
    double vRotatedx2 = cosr*Impl->ParticleN[index2].GetXVelocity() + sinr*Impl->ParticleN[index2].GetYVelocity();
    double vRotatedy2 = cosr*Impl->ParticleN[index2].GetYVelocity() - sinr*Impl->ParticleN[index2].GetXVelocity();

    double vGedrehttemp = vRotatedx1;
    vRotatedx1 = OneDcollision (vRotatedx1, vRotatedx2, Impl->ParticleN[index1].GetMass(), Impl->ParticleN[index2].GetMass());
    vRotatedx2 = OneDcollision (vRotatedx2, vGedrehttemp, Impl->ParticleN[index2].GetMass(), Impl->ParticleN[index1].GetMass());

    vGedrehttemp = vRotatedx1;
    vRotatedx1 = cosr * vRotatedx1 - sinr * vRotatedy1;
    vRotatedy1 = cosr * vRotatedy1 + sinr * vGedrehttemp;
    vGedrehttemp = vRotatedx2;
    vRotatedx2 = cosr * vRotatedx2 - sinr * vRotatedy2;
    vRotatedy2 = cosr * vRotatedy2 + sinr * vGedrehttemp;

    Impl->ParticleN[index1].SetVelocity(vRotatedx1, vRotatedy1);
    Impl->ParticleN[index2].SetVelocity(vRotatedx2, vRotatedy2);
}

void ParticleManager::AddForce(int index, double dfx, double dfy)
{
    Impl->ParticleN[index].IncreaseForce(dfx, dfy);
}
