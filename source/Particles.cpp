#include "Particles.h"
#include "ParticleManager.h"
#include "interfaces/IUserInput.h"
#include "interfaces/IParticlesView.h"
#include "ParticleDrawer.h"
#include "Vector.h"
#include "Particle.h"
#include "Physics.h"
#include "Config.h"
#include "interfaces/Rnd.h"
#include <iostream>

using namespace std;

Particles::Particles(IParticlesView &window, IUserInput & userInput, Config &parameters):
    W(window),
    UserInput (userInput),
    PManager(new ParticleManager(0)),
    Params(parameters),
    Drawer (new ParticleDrawer())
{
    UpdateBorderDimensions();
}

Particles::~Particles()
{
    delete Drawer;
    delete PManager;
}

void Particles::ApplyForce(int n1, int n2)
{
    Vector force = Physics::GetInterParticleForce(PManager->P(n1), PManager->P(n2), Params.PhysConstants);

    if ((Params.RestrictInterParticleForce || Params.WriteInterParticleForceWarning) && force.Abs() > Params.MaxInterParticleForce)
    {
        if (Params.WriteInterParticleForceWarning)
        {
            cout << "force between particle " << n1 << " and " << n2 << " is " << force.Abs() ;
            cout << endl;
        }

        if (Params.RestrictInterParticleForce)
        {
            force = force / force.Abs() * Params.MaxInterParticleForce;
            if (Params.WriteInterParticleForceWarning)
            {
                cout << "new force is: " << force.Abs() << endl ;
            }
        }
    }

    PManager->StoreForce(n1, n2, force);
    PManager->ResetNumSkippedForceCalculations(n1, n2);
}

void Particles::Draw() const
{
    Drawer->Draw(W, *PManager, Params);
}

void Particles::Sleep( clock_t wait ) {
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

void Particles::Init(void) {
    InitRandom();
    int MidX = Params.BorderDimensions.Get(0)/2;
    int MidY = Params.BorderDimensions.Get(1)/2;
    for (int n = 0; n < Params.ParticleCount; n++) {
        Vector velocity (0,0);
        Vector position (MidX, MidY);
        int positiveOrNegative = n%2 == 0 ? -1 : 1;
        double desiredParticleDistance = Params.InitialParticleDistance;
        if (Params.InitialPositioning == 0 || Params.InitialPositioning == 1)
        {
            double angle = 2.0 * M_PI / (double) Params.ParticleCount;

            double calculatedDistanceFromCenterOfMass = desiredParticleDistance / angle;
            position.Set(MidX + calculatedDistanceFromCenterOfMass*sin(angle*n), MidY + calculatedDistanceFromCenterOfMass*cos(angle*n));
            double speedRange =  0.22;
            Vector relativePosition = position - Vector(MidX, MidY);

            if (Params.InitialPositioning == 1)
            {
                if (relativePosition.Abs() > 0)
                {
                    velocity.Set(relativePosition.Y()/relativePosition.Abs()*speedRange, -relativePosition.X()/relativePosition.Abs()*speedRange);
                }
            }
        }
        else if (Params.InitialPositioning == 2)
        {
            position.Set(MidX + pow(n, 1.0/3.0)*desiredParticleDistance*sin(n), MidY + pow(n, 1.0/3.0)*desiredParticleDistance*cos(n));
            double speedRange =  0.01;
            velocity.Set(-0.5*speedRange+rnd(speedRange), - 0.5 * speedRange+rnd(speedRange));
        }
        else if (Params.InitialPositioning == 3)
        {
            position.Set(MidX + n * desiredParticleDistance, MidY);
            velocity.Set(0, 0);
        }

        PManager->AddParticle(position, velocity, Params.DefaultParticleMass, positiveOrNegative * Params.DefaultParticleCharge, Params.DefaultParticleRadius);
    }

    CheckParticlePositionsAreWithinWindow();

    if (Params.DoInteraction)
    {
        for (int n = 0; n < Params.ParticleCount; n++) {
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
    if (distance > Params.PhysConstants.AtomicRadius)
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
        if (distance < Params.PhysConstants.AtomicRadius)
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

void Particles::Update() {
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
                if (PManager->Distance(n1, n2) > (PManager->GetNumSkippedForceCalculations(n1, n2) + 1) * Params.PhysConstants.AtomicRadius * 2)
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
    for(int n1 = PManager->PCount() - 1; n1 >= 0; --n1) {
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
    if (CheckOverlap(index1, index2, distance)) {
        const Particle& p1 = PManager->P(index1);
        const Particle& p2 = PManager->P(index2);
        double minimumAllowedDistance = p1.Radius + p2.Radius;

        double factor = minimumAllowedDistance / distance;

        Vector distanceV = p2.Position - p1.Position;

        double cm = (p2.Mass * distance)/(p2.Mass + p1.Mass);  //center of mass distance from particle ..

        Vector centerOfMassPosition = p1.Position + (distanceV * (cm/distance));

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
        AddParticle(click.x*Params.Scale, click.y*Params.Scale, click.dx*Params.Scale, click.dy*Params.Scale);
    }

    bool windowSizeChanged = UserInput.CheckSizeChanged();
    if (windowSizeChanged)
    {
        UpdateBorderDimensions();
        CheckParticlePositionsAreWithinWindow();
    }

    if (check != 0 && check != '0')
    {
        cout << check << endl;
    }

    if (check =='r') {
        ReInit();
        cout << "Reset" << endl;
    } else if (check =='a') {
        RemoveParticle(PManager->P(PManager->PCount() - 1).Position.Get(0), PManager->P(PManager->PCount() - 1).Position.Get(1));
        cout << "Remove particle" << endl;
    } else if (check =='-') {
        Params.PhysConstants.GravitationalConstant /= 2;
        if (Params.PhysConstants.GravitationalConstant < 0.000000000001) Params.PhysConstants.GravitationalConstant = 0.0;
        cout << "G =" << Params.PhysConstants.GravitationalConstant << endl;
    }
    if (check =='+') {
        Params.PhysConstants.GravitationalConstant *= 2;
        if (Params.PhysConstants.GravitationalConstant < 0.000000000001) Params.PhysConstants.GravitationalConstant = 0.00000000001;
        cout << "G =" << Params.PhysConstants.GravitationalConstant << endl;
    } else if (check =='1') {
        Params.PhysConstants.ElectrostaticConstant /= 2;
        if (Params.PhysConstants.ElectrostaticConstant < 0.000000000001) Params.PhysConstants.ElectrostaticConstant = 0.0;
        cout << "E =" << Params.PhysConstants.ElectrostaticConstant << endl;
    } else if (check =='2') {
        Params.PhysConstants.ElectrostaticConstant *= 2;
        if (Params.PhysConstants.ElectrostaticConstant < 0.000000000001) Params.PhysConstants.ElectrostaticConstant = 0.00000000001;
        cout << "E =" << Params.PhysConstants.ElectrostaticConstant << endl;
    } else if (check =='4') {
        Params.PhysConstants.MolecularBondingEnergy /= 2;
        if (Params.PhysConstants.MolecularBondingEnergy < 0.000000000001) Params.PhysConstants.MolecularBondingEnergy = 0.0;
        cout << "Mol =" << Params.PhysConstants.MolecularBondingEnergy << endl;
    } else if (check =='5') {
        Params.PhysConstants.MolecularBondingEnergy *= 2;
        if (Params.PhysConstants.MolecularBondingEnergy < 0.000000000001) Params.PhysConstants.MolecularBondingEnergy = 0.00000000001;
        cout << "Mol =" << Params.PhysConstants.MolecularBondingEnergy << endl;
    } else if (check =='i') {
        Params.DoInteraction = !Params.DoInteraction;
        cout << "Particle Interaction set to " << Params.DoInteraction << endl;
    } else if (check =='v') {
        Params.DrawVelocities = !Params.DrawVelocities;
        cout << "Velocity drawing " << (Params.DrawVelocities?"enabled":"disabled") << endl;
    } else if (check =='e') {
        Params.ShowEnergies = !Params.ShowEnergies;
        cout << "Showing energies " << (Params.ShowEnergies?"enabled":"disabled") << endl;
    } else if (check =='/') {
        for (int i = PManager->PCount() -  1; i >= 0; --i) {
            PManager->ChangeRadius(i, -0.1);
        }
        cout << "radius reduced by 0.1; it is now " << PManager->P(0).Radius << endl;
    } else if (check =='*') {
        for (int i = PManager->PCount() -  1; i >= 0; --i) {
            PManager->ChangeRadius(i, 0.1);
        }
        cout << "radius increased by 0.1; it is now " << PManager->P(0).Radius << endl;
    }
}

void Particles::CheckParticlePositionsAreWithinWindow()
{
    for (int i = 0; i < PManager->PCount(); i++)
    {
        if (PManager->P(i).Position.X() > Params.BorderDimensions.X())
        {
            PManager->SetPosition(i, Vector(Params.BorderDimensions.X(), PManager->P(i).Position.Y()));
        }

        if (PManager->P(i).Position.Y() > Params.BorderDimensions.Y())
        {
            PManager->SetPosition(i, Vector(PManager->P(i).Position.X(), Params.BorderDimensions.Y()));
        }
    }
}

void Particles::UpdateBorderDimensions()
{
    Params.BorderDimensions.Set(W.GetSideX()* Params.Scale, W.GetSideY()*Params.Scale);
}
