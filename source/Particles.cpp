#include "Particles.h"
#include "ParticleManager.h"
#include "interfaces/IUserInput.h"
#include "interfaces/IParticlesView.h"
#include "Vector.h"
#include "Particle.h"
#include "Config.h"
#include "interfaces/Rnd.h"
#include <iostream>

using namespace std;



class ParticleDrawer
    {
    public:
        ParticleDrawer() = default;
        ~ParticleDrawer() = default;

        void Draw(IParticlesView& view, const ParticleManager& pm, Config& params)
        {
            view.ClearWindow(params.BackgroundColor);
            for (int i = 0; i < pm.PCount(); i++)
            {
                const Particle & p = pm.P(i);
                DrawParticle(view, pm, params, i);
                if (params.DrawVelocities)
                {
                    DrawVelocity(view, pm, params, i);
                }
            }
            view.DrawScreen();
        }

    private:
        void DrawParticle(IParticlesView& view, const ParticleManager& pm, Config& params, int index) const
        {
            const Particle& p = pm.P(index);

            int x = p.Position.X()/params.Scale;
            int y = p.Position.Y()/params.Scale;
            RGBData particleColor = params.UnchargedParticleColor;

            if(p.Charge>0.0)
            {
                particleColor = params.PositivelyChargedParticleColor;
            }
            else if (p.Charge<0.0)
            {
                particleColor = params.NegativeChargedParticleColor;
            }
            view.DrawParticle(index,particleColor, x,y);
        }

        void DrawVelocity(IParticlesView& view, const ParticleManager& pm, Config& params, int index) const
        {
            const Particle& p = pm.P(index);
            double rezScale = 1.0 / (double) params.Scale;
            double factor = params.VelocityLengthFactor;
            view.DrawLine(params.VelocityColor, p.Position.X() * rezScale , p.Position.Y() * rezScale, p.Velocity.X()*factor, p.Velocity.Y()*factor);
        }

    };


Particles::Particles(IParticlesView &window, IUserInput & userInput, Config &parameters):
    W(window),
    UserInput (userInput),
    PManager(new ParticleManager(0)),
    Params(parameters),
    Drawer (new ParticleDrawer())
{
    Params.BorderDimensions.Set(window.GetSideX()* Params.Scale, window.GetSideY()*Params.Scale);
}

Particles::~Particles()
{
    delete Drawer;
    delete PManager;
}

void Particles::ApplyForce(int n1, int n2)
{
    if (n1 == n2)
    {
        std::cout << "!!!!!!!!!!!!  error n1:" << n1 << "; n2: " << n2 << " !!!!!!!!!!!!!!!!" << std::endl;
        return;
    }

    double dx = PManager->P(n1).Position.Get(0) - PManager->P(n2).Position.Get(0);
    double dy = PManager->P(n1).Position.Get(1) - PManager->P(n2).Position.Get(1);
    double distance = sqrt (dx * dx + dy * dy);
    double reziR = 1.0 / distance;
    double reziR3 = reziR * reziR * reziR;


    // Gravity
    Vector gravity;
    if (Params.GravitationalConstant != 0.0)
    {
        double pot = - Params.GravitationalConstant * PManager->P(n1).Mass * PManager->P(n2).Mass * reziR3;
        double fgx = pot * dx;
        double fgy = pot * dy;
        gravity = Vector (fgx, fgy);
    }

    // Coulomb and Lorentz
     Vector electromagnetic;
    if (Params.ElectrostaticConstant != 0.0 || Params.MagneticPermeability != 0.0)
    {
        double pot = PManager->P(n1).Charge * PManager->P(n2).Charge * reziR3;
        double lorentz = PManager->P(n2).Velocity.Get(0) * dy - PManager->P(n2).Velocity.Get(1) * dx;
        double fcx = pot * (Params.ElectrostaticConstant * dx + Params.MagneticPermeability * PManager->P(n1).Velocity.Get(1) * lorentz );
        double fcy = pot * (Params.ElectrostaticConstant  *dy - Params.MagneticPermeability * PManager->P(n1).Velocity.Get(0) * lorentz );
        electromagnetic = Vector (fcx, fcy);
    }

    // Lennard-Jones Potential Force
    Vector molecular;
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

void Particles::Draw() const
{
    Drawer->Draw(W, *PManager, Params);
//    W.ClearWindow(Params.BackgroundColor);
//    for (int i = 0; i < PManager->PCount(); i++)
//    {
//        const Particle & p = PManager->P(i);
//        DrawParticle(i);
//        if (Params.DrawVelocities)
//        {
//            DrawVelocity(i);
//        }
//    }
//    W.DrawScreen();
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
    int MidX = Params.BorderDimensions.Get(0)/2;
    int MidY = Params.BorderDimensions.Get(1)/2;
    for (int n = 0; n < Params.ParticleCount; n++){
        int positiveOrNegative = n%2 == 0 ? -1 : 1;
        double distance = 30.0;
        Vector position(MidX + pow(n, 1.0/3.0)*distance*Params.Scale*sin(n), MidY + pow(n, 1.0/3.0)*distance*Params.Scale*cos(n));
        double speedRange =  0.01;
        Vector velocity (-0.5*speedRange+rnd(speedRange), - 0.5 * speedRange+rnd(speedRange));
        PManager->AddParticle(position, velocity, Params.DefaultParticleMass, positiveOrNegative * Params.DefaultParticleCharge, Params.DefaultParticleRadius);
    }

    if (Params.DoInteraction)
    {
        for (int n = 0; n < Params.ParticleCount; n++){
            for (int n2 = 0; n2 < n; ++n2)
            {
                ApplyForce(n,n2);
            }
        }
    }

    Draw();
}

void Particles::AddParticle(int x, int y, double dx, double dy)
{
    Vector velocity(dx/500.0, dy/500.0);
    Vector position (x,y);
    int closestParticle = PManager->GetClosestParticle(x,y);
    double distance = (position - PManager->P(closestParticle).Position).Abs();
    if (distance > Params.AtomicRadius)
    {
        PManager->AddParticle(position, velocity, Params.DefaultParticleMass, 0, Params.DefaultParticleRadius);
    }
    else
    {
        cout << "Can not add particle there are other particles too close to it" << endl;
    }
}


bool Particles::RemoveParticle(int x, int y)
{
    bool removed = false;
    if (PManager->PCount() > 0)
    {
        Vector position (x,y);
        int closestParticle = PManager->GetClosestParticle(x,y);
        double distance = (position - PManager->P(closestParticle).Position).Abs();
        if (distance < Params.AtomicRadius)
        {
            PManager->RemoveParticle(closestParticle);
            removed = true;
        }
    }

    if (removed)
    {
        Draw();
    }

    return removed;
}

void Particles::Update(){
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

    UpdateParticlesPositions();

    Draw();
}

void Particles::UpdateParticlesForcesAndVelocities()
{
    if (Params.DoInteraction)
    {
        for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
        {
            for (int n2 = n1-1; n2 >= 0; --n2)
            {
                if (PManager->Distance(n1, n2) > (PManager->GetNumSkippedForceCalculations(n1, n2) + 1) * Params.AtomicRadius * 2)
                {
                    PManager->SkipForceCalculation(n1, n2);
                }
                else
                {
                    ApplyForce(n1,n2);
                }
            }
            if (Params.ResetOnMaxAllowedForce && PManager->GetFullForce(n1).Abs() > Params.MaxAllowedForce)
            {
                cout << "Force on particle " << n1 << " is " << PManager->GetFullForce(n1).Abs() << "; resetting..." << endl;
                ReInit();
                return;
            }
        }
    }

    for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
    {
        PManager->UpdateVelocity(n1, Params.BorderDimensions, Params.Dissipation);
    }
}

void Particles::UpdateParticlesPositions()
{
    for(int n1 = PManager->PCount() - 1; n1 >= 0; --n1){
        PManager->UpdatePosition(n1);
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
                        PManager->PerformCollision(n1, n2);
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
    if (click.rightclick)
    {
        RemoveParticle(click.x*Params.Scale, click.y*Params.Scale);
    }
    else if (click.leftclick)
    {
        AddParticle(click.x*Params.Scale, click.y*Params.Scale, click.dx, click.dy);
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
    if (check =='v') {Params.DrawVelocities = !Params.DrawVelocities; cout << "Velocity drawing " << (Params.DrawVelocities?"enabled":"disabled") << endl;}
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
