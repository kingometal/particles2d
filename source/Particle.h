#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
private:
    double Position[2];
    double Velocity[2];
    double SavedPosition[2];

    double Mass;
    double ReciMass;
    double Charge;
    double Radius;
    double Force[2];

public:
    Particle();

    void UpdateSpeedAccordingToForce();
    void UpdateSpeedAccordingToForceDissipationAndBorders(double *maxCoord, double dissipation);

    void MoveAccordingToCurrentVelocity();

    void SetForce(double newGx, double newGy);
    void DecreaseForce(double minusDeltaFx, double minusDeltaFy);
    void IncreaseForce(double deltaFx, double deltaFy);
    double GetForce(int dim);

    void SetVelocity(double newVx, double newVy);
    double GetSquaredVelocity();

    void SetPosition(double newX, double newY);
    double GetYPosition();
    double GetXPosition();

    void SetCharge(double newQ);
    double GetCharge();

    void SetMass(double newM);
    double GetMass();

    double GetRadius();
    void SetRadius(double newR);

    bool CheckCollisionImminent(const Particle p2);

    double GetYVelocity();
    double GetXVelocity();


    // IParticle interface
public:
    double GetVelocity(int dim);
    double GetPosition(int dim);
    void UpdateVelocity();
    double GetDistance(const Particle &p2);
};
#endif
