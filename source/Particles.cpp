#include "Particles.h"
#include "ParticleManager.h"
#include "Constants.h"
#include "interfaces/Rnd.h"
#include <iostream>
#include "interfaces/IUserInput.h"
#include <cmath>
#include "interfaces/ParticlesViewInterface.h"
#include "Vector.h"
#include "Particle.h"
#include <iostream>


Particles::Particles(ParticlesViewInterface &window, IUserInput & userInput):
    W(window),
    UserInput (userInput),
    PManager(new ParticleManager(0)),
    GravitationalConstant(GRAVITY),
    ElectrostaticConstant(ELECTRO),
    MagneticPermeability(MAGNETI),
    MolecularBondingEnergy(ATOMIC)
  , BorderDimensions()
  , DoInteraction(true)

{
    BorderDimensions.Set(window.GetSideX()*ZOOM, window.GetSideY()*ZOOM);
}

Particles::~Particles()
{
    delete PManager;
}

Vector Particles::ApplyForce(int n1, int n2){
    double fgx=0.0, fgy=0.0, fcx = 0.0, fcy = 0.0, fmx = 0.0, fmy = 0.0, fTotX = 0.0, fTotY = 0.0;
    double dx = PManager->P(n1).Position.Get(0) - PManager->P(n2).Position.Get(0);
    double dy = PManager->P(n1).Position.Get(1) - PManager->P(n2).Position.Get(1);
    double distance = sqrt (dx * dx + dy * dy);
    double reziR = 1.0 / distance;
    double reziR3 = reziR * reziR * reziR;


    // Gravity
    {
        double pot = - GravitationalConstant * PManager->P(n1).Mass * PManager->P(n2).Mass * reziR3;
        fgx = pot*dx;
        fgy = pot*dy;
    }

    // Coulomb and Lorentz
    if (ElectrostaticConstant > 0.0)
    {
        double pot = PManager->P(n1).Charge * PManager->P(n2).Charge * reziR3;
        double lorentz = PManager->P(n2).Velocity.Get(0) * dy - PManager->P(n2).Velocity.Get(1) * dx;
        fcx = pot * (ElectrostaticConstant * dx + MagneticPermeability * PManager->P(n1).Velocity.Get(1) * lorentz );
        fcy = pot * (ElectrostaticConstant  *dy - MagneticPermeability * PManager->P(n1).Velocity.Get(0) * lorentz );
    }

    // Lennard-Jones Potential Force
    if (distance < ATOMIC_RADIUS * 20)
    {
        double arr = pow(ATOMIC_RADIUS*reziR , 6);
        double pot = MolecularBondingEnergy * (pow(arr, 2) - arr) * reziR ;
        fmx = pot * dx;
        fmy = pot * dy;
    }

    fTotX = fgx+fcx+fmx;
    fTotY = fgy+fcy+fmy;

    PManager->AddForce(n1, fTotX, fTotY);
    PManager->AddForce(n2, -fTotX, -fTotY);

    Vector resultingForce (fTotX, fTotY);
    if (resultingForce.Abs() > 10)
    {
        cout << "force between " << n1 << " and " << n2 << " is " << resultingForce.Abs() ;
        cout << "; distance between " << n1 << " and " << n2 << " is " << distance ;
        Vector gravity (fgx, fgy);
        Vector electromagnetic (fcx, fcy);
        Vector molecular (fmx, fmy);
        cout << "; gravity is " << gravity.Abs() ;
        cout << "; electromagnetic force is " << electromagnetic.Abs() ;
        cout << "; molecular force is " << molecular.Abs() ;
        cout << endl;
    }
    return resultingForce;
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
    int x = newPosition.v[0]/ZOOM;
    int y = newPosition.v[1]/ZOOM;
    int oldX = oldPosition.v[0]/ZOOM;
    int oldY = oldPosition.v[1]/ZOOM;

    EColor particleColor = Black;
    if(q>0.0)
    {
        particleColor = Red;
    }
    else if (q<0.0)
    {
        particleColor = Blue;
    }
    W.DrawParticle(index, White, oldX, oldY);
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
    W.ClearWindow();
    int MidX = BorderDimensions.Get(0)/2;
    int MidY = BorderDimensions.Get(1)/2;
    for (int n = 0; n < PARTICLE_COUNT; n++){
        int positiveOrNegative = n%2 == 0 ? -1 : 1;
        double distance = 30.0;
        Vector position(MidX + pow(n, 1.0/3.0)*distance*ZOOM*sin(n), MidY + pow(n, 1.0/3.0)*distance*ZOOM*cos(n));
        double speedRange =  0.01;
        Vector velocity (-0.5*speedRange+rnd(speedRange), - 0.5 * speedRange+rnd(speedRange));
        int newParticleCount = PManager->AddParticle(position, velocity, MASS, positiveOrNegative * CHARGE, RADIUS);
        const Particle& p = PManager->P(newParticleCount - 1);
        RedrawParticleAtNewPosition(n, p.Position, p.Position, p.Charge);
    }
    W.DrawScreen();
}

void Particles::AddParticle(int x, int y, double dx, double dy)
{
    Vector position(x,y);
    Vector velocity(dx/500.0, dy/500.0);
    PManager->AddParticle(position, velocity, MASS, 0, RADIUS);
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
        if (distance < ATOMIC_RADIUS)
        {
            W.DrawParticle(closestParticle, White, PManager->P(closestParticle).Position.Get(0), PManager->P(closestParticle).Position.Get(1));
            PManager->RemoveParticle(closestParticle);
            W.ClearWindow();
            removed = true;
//            cout << "removed" << endl;
        }
    }
    return removed;
}

void Particles::Update(){
    PManager->ResetForces();
    HandleKeyPress();
#if CHECK_COLLISIONS
    if (DoInteraction)
    {
        AvoidCollisions();
    }
#endif

    UpdateParticlesForcesAndVelocities();
#if CHECK_COLLISIONS
    if (DoInteraction)
    {

        AvoidCollisions();

    }
#endif
    UpdateParticlesPositionsAndDraw();
    W.DrawScreen();
}

void Particles::UpdateParticlesForcesAndVelocities()
{
    for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
    {
        if (DoInteraction)
        {
            for (int n2 = n1-1; n2 >= 0; --n2) // this has to be done in this order, not "n2 = 0; n2 < n1;" because otherwise you cannot update speed in the same loop
            {
                ApplyForce(n1,n2);
            }
        }
        if (PManager->P(n1).Force.Abs() > 100*ZOOM)
        {
            cout << "force of " << n1 << " is " << PManager->P(n1).Force.Abs() << endl;
            cout << "its neghbours are: " << endl;
            for (int i = 0; i < PManager->PCount(); i++)
            {
                Vector force = ApplyForce(n1, i);
                double distance = PManager->Distance(n1, i);
                if (distance < ATOMIC_RADIUS || force.Abs() > 0.1)
                {
                    cout << i << "; distance: " << distance << "; force: " << force.Abs() << endl;
                }
            }
            ReInit();
            return;
        }
        PManager->UpdateVelocity(n1, BorderDimensions, DISSIPATION);
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
                if (++retries > RESOLVE_COLLISIONS_RETRIES)
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
                        PManager->UpdateVelocity(n1, BorderDimensions, DISSIPATION);
                        PManager->UpdateVelocity(n2, BorderDimensions, DISSIPATION);
                    }
                }
            }
        }
        if (success) break;
    }
    while (outerRetries++ < 1);

    if (outerRetries >= RESOLVE_COLLISIONS_RETRIES)
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
        if (! RemoveParticle(click.x*ZOOM, click.y*ZOOM))
        {
            AddParticle(click.x*ZOOM, click.y*ZOOM, click.dx, click.dy);
        }
    }
    if (check != 0 && check != '0')
    {
        cout << check << endl;
    }
    if (check =='r') {ReInit();  cout << "Reset" << endl;}
    if (check =='a') {RemoveParticle(PManager->P(PManager->PCount() - 1).Position.Get(0), PManager->P(PManager->PCount() - 1).Position.Get(1)); cout << "Remove particle" << endl;}
    if (check =='-') {GravitationalConstant /= 2; if (GravitationalConstant < 0.000000000001) GravitationalConstant = 0.0;  cout << "G =" << GravitationalConstant << endl;}
    if (check =='+') {GravitationalConstant *= 2; if (GravitationalConstant < 0.000000000001) GravitationalConstant = 0.00000000001;  cout << "G =" << GravitationalConstant << endl;}
    if (check =='1') {ElectrostaticConstant /= 2; if (ElectrostaticConstant < 0.000000000001) ElectrostaticConstant = 0.0;  cout << "E =" << ElectrostaticConstant << endl;}
    if (check =='2') {ElectrostaticConstant *= 2; if (ElectrostaticConstant < 0.000000000001) ElectrostaticConstant = 0.00000000001;  cout << "E =" << ElectrostaticConstant << endl;}
    if (check =='4') {MolecularBondingEnergy /= 2; if (MolecularBondingEnergy < 0.000000000001) MolecularBondingEnergy = 0.0;  cout << "Mol =" << MolecularBondingEnergy << endl;}
    if (check =='5') {MolecularBondingEnergy *= 2; if (MolecularBondingEnergy < 0.000000000001) MolecularBondingEnergy = 0.00000000001;  cout << "Mol =" << MolecularBondingEnergy << endl;}
    if (check =='i') {DoInteraction = !DoInteraction;  cout << "Particle Interaction set to " << DoInteraction << endl;}

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
