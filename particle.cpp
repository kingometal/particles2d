#include "particle.h"
#include "constants.h"


Particle::Particle(void){
        for (int n = 0; n< 2; n++){
                Position[n] = 0;
                Velocity[n] = 0;
                Force[n] = 0;
        }
        Mass=MASS;
        Charge=CHARGE;
        Radius=RADIUS;
}


void  Particle::SetRadius(double newR){
        Radius = newR;
}

double Particle::GetRadius(){
        return Radius;
}

double Particle::GetMass(){
        return Mass;
}

void Particle::SetMass(double newM){
        Mass = newM;
}

double Particle::GetCharge(){
        return Charge;
}

void Particle::SetCharge(double newQ){
        Charge = newQ;
}

double Particle::GetXPosition(void){
        return Position[0];
}

double Particle::GetYPosition(void){
        return Position[1];
}

double Particle::GetSavedXPosition(void){
        return SavedPosition[0];
}

double Particle::GetSavedYPosition(void){
        return SavedPosition[1];
}


void Particle::SetPosition(double newX, double newY){
        Position[0] = newX;
        Position[1] = newY;
}

void Particle::SavePosition(){
        SavedPosition[0] = Position[0];
        SavedPosition[1] = Position[1];
}

double Particle::GetXVelocity(void){
        return Velocity[0];
}

double Particle::GetYVelocity(void){
        return Velocity[1];
}

double Particle::GetSquaredVelocity(void){
        return Velocity[0]*Velocity[0]+Velocity[1]*Velocity[1];
}

void Particle::SetVelocity(double newVx, double newVy){
        Velocity[0] = newVx;
        Velocity[1] = newVy;
}

void Particle::SetForce(double newGx, double newGy){
        Force[0] = newGx;
        Force[1] = newGy;

}

void Particle::IncreaseForce(double deltaFx, double deltaFy){
        Force[0] += deltaFx;
        Force[1] += deltaFy;
}

void Particle::DecreaseForce(double minusDeltaFx, double minusDeltaFy){
        Force[0] -= minusDeltaFx;
        Force[1] -= minusDeltaFy;
}

void Particle::MoveAccordingToCurrentVelocity(){
        Position[0]+=Velocity[0];
        Position[1]+=Velocity[1];
}

void Particle::MoveAccordingToCurrentVelocity(double L){
        Position[0]+=Velocity[0];
        Position[1]+=Velocity[1];
}


void Particle::UpdateSpeedAccordingToForceDissipationAndBorders(double L){
        for (int n=0; n<2 ;n++){
                if (Position[n]+Velocity[n]+Force[n]/Mass >= L or Position[n]+Velocity[n]+Force[n]/Mass <0) {Velocity[n]+= Force[n]/Mass; Velocity[n] *= -1;}
                Velocity[n] += Force[n]/Mass;
                Velocity[n] = Velocity[n]*(1 - 0.01*DISSIPATION_PERCENT);
        }
}

void Particle::UpdateSpeedAccordingToForce(){
        for (int n=0; n<2 ;n++){
                Velocity[n] += Force[n]/Mass;
        }
}
