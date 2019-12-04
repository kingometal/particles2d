#include "Particles.h"
#include "ParticleManager.h"
#include "interfaces/Rnd.h"
#include <iostream>
#include "interfaces/IUserInput.h"
#include <cmath>
#include "interfaces/ParticlesViewInterface.h"
#include "Vector.h"
#include "Particle.h"
#include "Config.h"
#include <iostream>

#define MAX_FLOAT 3.40282347e+38F

Particles::Particles(ParticlesViewInterface &window, IUserInput & userInput, Config &parameters):
    W(window),
    UserInput (userInput),
    PManager(new ParticleManager(0)),
    Params(parameters)
{
    Params.BorderDimensions.Set(window.GetSideX()* Params.Scale, window.GetSideY()*Params.Scale);
}

Particles::~Particles()
{
    delete PManager;
}

void Particles::ApplyForce(int n1, int n2){
    if (n1 == n2)
    {
        std::cout << "!!!!!!!!!!!!  error n1:" << n1 << "; n2: " << n2 << " !!!!!!!!!!!!!!!!" << std::endl;
        return;
    }

    double dx = PManager->P(n1).Position.Get(0) - PManager->P(n2).Position.Get(0);
    double dy = PManager->P(n1).Position.Get(1) - PManager->P(n2).Position.Get(1);
    double distance = sqrt (dx * dx + dy * dy);

    if ( ( distance > (PManager->GetNumSkippedForceCalculations(n1, n2) + 1) * Params.AtomicRadius * 1) && PManager->P(n1).Forces.count(n2))
    {
        PManager->SkipForceCalculation(n1, n2);
        return;
    }


    Vector gravity, electromagnetic, molecular;
    double reziR = 1.0 / distance;
    double reziR3 = reziR * reziR * reziR;


    // Gravity
    if (Params.GravitationalConstant != 0.0)
    {
        double pot = - Params.GravitationalConstant * PManager->P(n1).Mass * PManager->P(n2).Mass * reziR3;
        double fgx = pot * dx;
        double fgy = pot * dy;
        gravity = Vector (fgx, fgy);
    }

    // Coulomb and Lorentz
    if (Params.ElectrostaticConstant != 0.0 || Params.MagneticPermeability != 0.0)
    {
        double pot = PManager->P(n1).Charge * PManager->P(n2).Charge * reziR3;
        double lorentz = PManager->P(n2).Velocity.Get(0) * dy - PManager->P(n2).Velocity.Get(1) * dx;
        double fcx = pot * (Params.ElectrostaticConstant * dx + Params.MagneticPermeability * PManager->P(n1).Velocity.Get(1) * lorentz );
        double fcy = pot * (Params.ElectrostaticConstant  *dy - Params.MagneticPermeability * PManager->P(n1).Velocity.Get(0) * lorentz );
        electromagnetic = Vector (fcx, fcy);
    }

    // Lennard-Jones Potential Force
    if (Params.MolecularBondingEnergy != 0 && distance < Params.AtomicRadius * 10)
    {
        double arr = pow(Params.AtomicRadius*reziR , 6);
        double pot = Params.MolecularBondingEnergy * (pow(arr, 2) - arr) * reziR ;
        double fmx = pot * dx;
        double fmy = pot * dy;
        molecular = Vector (fmx, fmy);
    }

    Vector resultingForce = gravity + electromagnetic + molecular;

    if ((Params.RestrictInterParticleForce || Params.WriteInterParticleForceWarning) && resultingForce.Abs() > Params.MaxInterParticleForce)
    {
        if (Params.WriteInterParticleForceWarning)
        {
            cout << "force between " << n1 << " and " << n2 << " is " << resultingForce.Abs() ;
            cout << "; distance is " << distance ;
            cout << "; gravity is " << gravity.Abs() ;
            cout << "; electromagnetic force is " << electromagnetic.Abs() ;
            cout << "; molecular force is " << molecular.Abs() ;
            cout << endl;
        }

        if (Params.RestrictInterParticleForce)
        {
            resultingForce = resultingForce / resultingForce.Abs() * Params.MaxInterParticleForce;
            if (Params.WriteInterParticleForceWarning)
            {
                cout << "new force is: " << resultingForce.Abs() << endl ;
            }
        }
    }

    PManager->StoreForce(n1, n2, resultingForce);
    PManager->ResetNumSkippedForceCalculations(n1, n2);

    return;
}

//double Particles::eKin(){
//    double En = 0.0;
//    for (int n1 = 0; n1 < NumParticles; n1++){
//        En += 0.5*ParticleN[n1].GetMass()*ParticleN[n1].GetSquaredVelocity();
//    }
//    return(En);
//}

//double Particles::eEl(){
//    double En = 0.0;
//    for (int n1 = 0; n1 < NumParticles; n1++){
//        for(int n2=n1+1; n2<NumParticles; n2++){
//            En+=ElectrostaticConstant*ParticleN[n1].GetCharge()*ParticleN[n2].GetCharge()/getR(n1,n2);
//        }
//    }
//    return(En);
//}

//double Particles::eG(){
//    double En = 0.0;
//    for (int n1 = 0; n1 < NumParticles; n1++){
//        for(int n2=n1+1; n2<NumParticles; n2++){
//            En=GravitationalConstant*ParticleN[n1].GetMass()*ParticleN[n2].GetMass()/getR(n1,n2);
//        }
//    }
//    return(En);
//}

//double Particles::eMol(){
//    double En = 0.0;
//    for (int n1 = 0; n1 < NumParticles; n1++){
//        for(int n2=n1+1; n2<NumParticles; n2++){
//            double r = getR(n1,n2);
//            En-= 4*MolecularBondingEnergy * (pow(ATOMIC_RADIUS/r, 12) - pow(ATOMIC_RADIUS/r, 6)) ;
//        }
//    }
//    return(En);
//}


void Particles::RedrawParticleAtNewPosition(int index, const Vector& oldPosition, const Vector& newPosition, double q)
{
    int x = newPosition.v[0]/Params.Scale;
    int y = newPosition.v[1]/Params.Scale;
    int oldX = oldPosition.v[0]/Params.Scale;
    int oldY = oldPosition.v[1]/Params.Scale;

    RGBData particleColor = Params.UnchargedParticleColor;
    if(q>0.0)
    {
        particleColor = Params.PositivelyChargedParticleColor;
    }
    else if (q<0.0)
    {
        particleColor = Params.NegativeChargedParticleColor;
    }
    W.DrawParticle(index, Params.BackgroundColor, oldX, oldY);
    W.DrawParticle(index,particleColor, x,y);
}

//void Particles::RedrawParticleAtNewPosition(int index)
//{
//    RedrawParticleAtNewPosition(index, ParticleN[index].GetSavedXPosition(), ParticleN[index].GetSavedYPosition(), ParticleN[index].GetXPosition(), ParticleN[index].GetYPosition(), Charge(index));
//}

void Particles::RedrawV(){
    //    W.Black();
    //    W.DrawLine (x, y, x+vx*100, y+vy*100);
}

void Particles::RedrawV(int color){
    //    W.Black();
    //    if (color ==0) W.White();
    //    W.DrawLine (x, y, x+vx*100, y+vy*100);
    //    W.Black();
}

void Particles::Sleep( clock_t wait ){
    clock_t goal;
    goal = wait + clock();
    while( goal > clock() );
}

void Particles::ReInit(void)
{
    while (PManager->PCount() > 0)
    {
        PManager->RemoveParticle(0);
    }
    Init();
}

void Particles::Init(void){
    InitRandom();
    W.ClearWindow(Params.BackgroundColor);
    int MidX = Params.BorderDimensions.Get(0)/2;
    int MidY = Params.BorderDimensions.Get(1)/2;
    for (int n = 0; n < Params.ParticleCount; n++){
        int positiveOrNegative = n%2 == 0 ? -1 : 1;
        double distance = 30.0;
        Vector position(MidX + pow(n, 1.0/3.0)*distance*Params.Scale*sin(n), MidY + pow(n, 1.0/3.0)*distance*Params.Scale*cos(n));
        double speedRange =  0.01;
        Vector velocity (-0.5*speedRange+rnd(speedRange), - 0.5 * speedRange+rnd(speedRange));
        int newParticleCount = PManager->AddParticle(position, velocity, Params.DefaultParticleMass, positiveOrNegative * Params.DefaultParticleCharge, Params.DefaultParticleRadius);
        const Particle& p = PManager->P(newParticleCount - 1);
        RedrawParticleAtNewPosition(n, p.Position, p.Position, p.Charge);
    }
    W.DrawScreen();
}

void Particles::AddParticle(int x, int y, double dx, double dy)
{
    Vector position(x,y);
    Vector velocity(dx/500.0, dy/500.0);
    PManager->AddParticle(position, velocity, Params.DefaultParticleMass, 0, Params.DefaultParticleRadius);
}

int Particles::GetClosestParticle(int x, int y)
{
    int closestParticle = -1;
    double shortestDistance = MAX_FLOAT;
    Vector position(x,y);
    for (int i = PManager->PCount() -  1; i >= 0; --i)
    {
        double distance = (position - PManager->P(i).Position).Abs();
        if (distance < shortestDistance)
        {
            closestParticle = i;
            shortestDistance = distance;
        }
    }
    return closestParticle;
}

bool Particles::RemoveParticle(int x, int y)
{
    bool removed = false;
    if (PManager->PCount() > 0)
    {
        Vector position (x,y);
        int closestParticle = GetClosestParticle(x,y);
        double distance = (position - PManager->P(closestParticle).Position).Abs();
        if (distance < Params.AtomicRadius)
        {
            W.DrawParticle(closestParticle, Params.BackgroundColor, PManager->P(closestParticle).Position.Get(0), PManager->P(closestParticle).Position.Get(1));
            PManager->RemoveParticle(closestParticle);
            W.ClearWindow(Params.BackgroundColor);
            removed = true;
//            cout << "removed" << endl;
        }
    }
    return removed;
}

void Particles::Update(){
    PManager->ResetForces();
    HandleKeyPress();
    if (Params.CheckCollisions && Params.DoInteraction)
    {
        AvoidCollisions();
    }

    UpdateParticlesForcesAndVelocities();
    if (Params.CheckCollisions && Params.DoInteraction)
    {
        AvoidCollisions();
    }
    UpdateParticlesPositionsAndDraw();
    W.DrawScreen();
}

void Particles::UpdateParticlesForcesAndVelocities()
{
    for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
    {
        if (Params.DoInteraction)
        {
            for (int n2 = n1-1; n2 >= 0; --n2) // this has to be done in this order, not "n2 = 0; n2 < n1;" because otherwise you cannot update speed in the same loop
            {
                ApplyForce(n1,n2);
            }
        }
        if (Params.ResetOnMaxAllowedForce && PManager->P(n1).Force.Abs() > Params.MaxAllowedForce)
        {
            cout << "Force on particle " << n1 << " is " << PManager->P(n1).Force.Abs() << "; resetting..." << endl;
            ReInit();
            return;
        }
    }

    for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
    {
        PManager->UpdateVelocity(n1, Params.BorderDimensions, Params.Dissipation);
    }
}

void Particles::UpdateParticlesPositionsAndDraw()
{
    Vector oldPosition;
    for(int n1 = PManager->PCount() - 1; n1 >= 0; --n1){
        oldPosition = PManager->P(n1).Position;
        PManager->UpdatePosition(n1);
        RedrawParticleAtNewPosition(n1, oldPosition, PManager->P(n1).Position, PManager->P(n1).Charge);
    }
}

void Particles::AvoidCollisions()
{
    int outerRetries = 0;
    do
    {
        int success = true;
        for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
        {
            int retries = 0;
            bool doCollisions = true;
            while (doCollisions)
            {
                if (++retries > Params.ResolveCollisionsRetries)
                {
//                    cout << "timeout calculating collisions of " << n1 << endl;
                    success = false;
                    break;
                }
                doCollisions = false;
                for (int n2 = n1 - 1; n2 >= 0; --n2)
                {
                    double distance = PManager->Distance(n1,n2);
                    ResolveOverlapIfNeeded(n1,n2, distance);
                    if (CheckCollisionImminent(n1, n2))
                    {
                        doCollisions = true;
//                        cout << "collision between " << n1 << " and " << n2 << endl;
                        PManager->PerformCollision(n1, n2);
                        PManager->ResetForce(n1);
                        PManager->ResetForce(n2);
                        ApplyForce(n1, n2);
                        PManager->UpdateVelocity(n1, Params.BorderDimensions, Params.Dissipation);
                        PManager->UpdateVelocity(n2, Params.BorderDimensions, Params.Dissipation);
                    }
                }
            }
        }
        if (success) break;
    }
    while (outerRetries++ < 1);

    if (outerRetries >= Params.ResolveCollisionsRetries)
    {
//        cout << "timeout calculating overall collisions" << endl;
    }
}

bool Particles::CheckOverlap(const int index1, const int index2, const double distance) const
{
    return distance < PManager->P(index1).Radius + PManager->P(index2).Radius;
}

void Particles::ResolveOverlapIfNeeded(int index1, int index2, double distance)
{
    if (CheckOverlap(index1, index2, distance)){
        const Particle& p1 = PManager->P(index1);
        const Particle& p2 = PManager->P(index2);
        double minimumAllowedDistance = p1.Radius + p2.Radius;

        double factor = minimumAllowedDistance / distance;
        /*
        p2.Position = p1.Position + ((p2.Position - p1.Position) * factor);
        */

        Vector distanceV = p2.Position - p1.Position;

        double cm = (p2.Mass * distance)/(p2.Mass + p1.Mass);  //center of mass distance from particle ..

        Vector centerOfMassPosition = p1.Position + (distanceV * (cm/distance));


        Vector OldPosition1 = p1.Position;
        Vector OldPosition2 = p2.Position;

        PManager->SetPosition(index2, (p2.Position - centerOfMassPosition) * factor + centerOfMassPosition);
        PManager->SetPosition(index1, (p1.Position - centerOfMassPosition) * factor + centerOfMassPosition);

        RedrawParticleAtNewPosition(index1, OldPosition1, p1.Position, p1.Charge);
        RedrawParticleAtNewPosition(index2, OldPosition2, p2.Position, p2.Charge);
    }
}


bool Particles::CheckCollisionImminent(const int index1, const int index2) const
{
    const Particle& p1 = PManager->P(index1);
    const Particle& p2 = PManager->P(index2);
    return ((p2.Position + p2.Velocity) - (p1.Position + p1.Velocity)).Abs() < p2.Radius + p1.Radius;
}

void Particles::HandleKeyPress()
{
    char check = UserInput.CheckKeyPress();
    MouseClick click = UserInput.CheckMouseClick();
    if (click.click)
    {
        if (! RemoveParticle(click.x*Params.Scale, click.y*Params.Scale))
        {
            AddParticle(click.x*Params.Scale, click.y*Params.Scale, click.dx, click.dy);
        }
    }
    if (check != 0 && check != '0')
    {
        cout << check << endl;
    }
    if (check =='r') {ReInit();  cout << "Reset" << endl;}
    if (check =='a') {RemoveParticle(PManager->P(PManager->PCount() - 1).Position.Get(0), PManager->P(PManager->PCount() - 1).Position.Get(1)); cout << "Remove particle" << endl;}
    if (check =='-') {Params.GravitationalConstant /= 2; if (Params.GravitationalConstant < 0.000000000001) Params.GravitationalConstant = 0.0;  cout << "G =" << Params.GravitationalConstant << endl;}
    if (check =='+') {Params.GravitationalConstant *= 2; if (Params.GravitationalConstant < 0.000000000001) Params.GravitationalConstant = 0.00000000001;  cout << "G =" << Params.GravitationalConstant << endl;}
    if (check =='1') {Params.ElectrostaticConstant /= 2; if (Params.ElectrostaticConstant < 0.000000000001) Params.ElectrostaticConstant = 0.0;  cout << "E =" << Params.ElectrostaticConstant << endl;}
    if (check =='2') {Params.ElectrostaticConstant *= 2; if (Params.ElectrostaticConstant < 0.000000000001) Params.ElectrostaticConstant = 0.00000000001;  cout << "E =" << Params.ElectrostaticConstant << endl;}
    if (check =='4') {Params.MolecularBondingEnergy /= 2; if (Params.MolecularBondingEnergy < 0.000000000001) Params.MolecularBondingEnergy = 0.0;  cout << "Mol =" << Params.MolecularBondingEnergy << endl;}
    if (check =='5') {Params.MolecularBondingEnergy *= 2; if (Params.MolecularBondingEnergy < 0.000000000001) Params.MolecularBondingEnergy = 0.00000000001;  cout << "Mol =" << Params.MolecularBondingEnergy << endl;}
    if (check =='i') {Params.DoInteraction = !Params.DoInteraction;  cout << "Particle Interaction set to " << Params.DoInteraction << endl;}

    if (check =='/') {
        for (int i = PManager->PCount() -  1; i >= 0; --i){
            PManager->ChangeRadius(i, -0.1);
        }
        cout << "radius reduced by 0.1; it is now " << PManager->P(0).Radius << endl;
    }
    if (check =='*') {
        for (int i = PManager->PCount() -  1; i >= 0; --i){
            PManager->ChangeRadius(i, 0.1);
        }
        cout << "radius increased by 0.1; it is now " << PManager->P(0).Radius << endl;
    }
}
