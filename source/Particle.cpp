#include "Particle.h"
#include <cmath>

Particle::Particle(void)
    : Position()
    , Velocity()
    , Force()
    , Mass(1.0)
    , ReciMass(1.0 / Mass)
    , Charge(0.0)
    , Radius(1.0)
{
}

//void  Particle::SetRadius(double newR)
//{
//    Radius = newR;
//}

//double Particle::GetRadius()
//{
//    return Radius;
//}

//double Particle::GetMass()
//{
//    return Mass;
//}

//void Particle::SetMass(double newM)
//{
//    Mass = newM;
//    if (Mass == 0)
//    {
//        ReciMass = 3.40282347e+38F;
//    }
//    else
//    {
//        ReciMass = 1.0 / Mass;
//    }
//}

//double Particle::GetCharge()
//{
//    return Charge;
//}

//void Particle::SetCharge(double newQ)
//{
//    Charge = newQ;
//}

//double Particle::GetXPosition(void)
//{
//    return Position[0];
//}

//double Particle::GetYPosition(void)
//{
//    return Position[1];
//}

//void Particle::SetPosition(double newX, double newY)
//{
//    Position[0] = newX;
//    Position[1] = newY;
//}

//double Particle::GetXVelocity(void)
//{
//    return Velocity[0];
//}

//double Particle::GetVelocity(int dim)
//{
//    return Velocity[dim];
//}

//double Particle::GetPosition(int dim)
//{
//    return Position[dim];
//}

//void Particle::UpdateVelocity()
//{

//}

//double Particle::GetYVelocity(void)
//{
//    return Velocity[1];
//}

//double Particle::GetSquaredVelocity(void)
//{
//    return Velocity[0] * Velocity[0] + Velocity[1] * Velocity[1];
//}

//void Particle::SetVelocity(double newVx, double newVy)
//{
//    Velocity[0] = newVx;
//    Velocity[1] = newVy;
//}

//void Particle::SetForce(double newGx, double newGy)
//{
//    Force[0] = newGx;
//    Force[1] = newGy;
//}

//void Particle::IncreaseForce(double deltaFx, double deltaFy)
//{
//    Force[0] += deltaFx;
//    Force[1] += deltaFy;
//}

//void Particle::DecreaseForce(double minusDeltaFx, double minusDeltaFy)
//{
//    Force[0] -= minusDeltaFx;
//    Force[1] -= minusDeltaFy;
//}

//double Particle::GetForce(int dim)
//{
//    return Force[dim];
//}

//double Particle::GetDistance(const Particle& p2)
//{
//    return sqrt((p2.Position[0] - Position[0]) *
//                (p2.Position[0] - Position[0]) +
//                (p2.Position[1] - Position[1]) *
//                (p2.Position[1] - Position[1]));
//}

//void Particle::MoveAccordingToCurrentVelocity()
//{
//    Position[0] += Velocity[0];
//    Position[1] += Velocity[1];
//}

//void Particle::UpdateSpeedAccordingToForceDissipationAndBorders(double* maxCoord, double dissipation)
//{
//    double acceleration;
//    double nextPossiblePosition;
//    double newVelocity;

//    for (int n = 0 ; n < 2 ;n++)
//    {
//        acceleration = Force[n] * ReciMass;
//        newVelocity = Velocity[n];
//        nextPossiblePosition = Position[n] + newVelocity + acceleration;
//        if (nextPossiblePosition >= maxCoord[n] || nextPossiblePosition < 0)
//        {
//            newVelocity *= -1;
//        }
//        else
//        {
//            newVelocity += acceleration;
//        }
//        Velocity[n] = newVelocity * (1 - dissipation);
//    }
//}

//void Particle::UpdateSpeedAccordingToForce()
//{
//    Velocity[0] += Force[0] * ReciMass;
//    Velocity[1] += Force[1] * ReciMass;
//}

//bool Particle::CheckCollisionImminent(const Particle p2){
//    return sqrt(
//                ((p2.Position[0]+p2.Velocity[0]) - (Position[0]+Velocity[0])) *
//                ((p2.Position[0]+p2.Velocity[0]) - (Position[0]+Velocity[0])) +
//                ((p2.Position[1]+p2.Velocity[1]) - (Position[1]+Velocity[1])) *
//                ((p2.Position[1]+p2.Velocity[1]) - (Position[1]+Velocity[1]))
//                )
//            < p2.Radius + Radius;
//}
