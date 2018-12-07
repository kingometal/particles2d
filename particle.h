#ifndef PARTICLE_H
#define PARTICLE_H

class Particle{
private:
    double Position[2];
    double Velocity[2];
    double SavedPosition[2];

    double Mass;
    double Charge;
    double Radius;
    double Force[2];

public:
        Particle();
        void UpdateSpeedAccordingToForce();
        void UpdateSpeedAccordingToForceDissipationAndBorders(double L);
        void MoveAccordingToCurrentVelocity(double L);
        void MoveAccordingToCurrentVelocity();
        void DecreaseForce(double minusDeltaFx, double minusDeltaFy);
        void IncreaseForce(double deltaFx, double deltaFy);
        void SetForce(double newGx, double newGy);
        void SetVelocity(double newVx, double newVy);
        double GetSquaredVelocity();
        double GetYVelocity();
        double GetXVelocity();
        void SavePosition();
        void SetPosition(double newX, double newY);
        double GetSavedYPosition();
        double GetSavedXPosition();
        double GetYPosition();
        double GetXPosition();
        void SetCharge(double newQ);
        double GetCharge();
        void SetMass(double newM);
        double GetMass();
        double GetRadius();
        void SetRadius(double newR);
};
#endif
