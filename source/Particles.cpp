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
    PManager(new ParticleManager(PARTICLE_COUNT)),
    GravitationalConstant(GRAVITY),
    ElectrostaticConstant(ELECTRO),
    MagneticPermeability(MAGNETI),
    MolecularBondingEnergy(ATOMIC)
{
    BorderDimensions.Set(window.GetSideX()*ZOOM, window.GetSideY()*ZOOM);
}

Particles::~Particles()
{
    delete PManager;
}

void Particles::ApplyForce(int n1, int n2){
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


void Particles::RedrawParticleAtNewPosition(int index, double oldX, double oldY, double x, double y, double q)
{
    x = x/ZOOM;
    y = y/ZOOM;
    oldX = oldX/ZOOM;
    oldY = oldY/ZOOM;

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

void Particles::Init(void){
    InitRandom();
    W.ClearWindow();
    double R =  RADIUS;
    int MidX = BorderDimensions.Get(0)/2;
    int MidY = BorderDimensions.Get(1)/2;
    for (int n = PManager->PCount() - 1; n >= 0; --n){
        int positiveOrNegative = n%2 == 0 ? -1 : 1;
        double distance = 30.0;
        Vector position;
        position.v[0] = MidX + pow(n, 1.0/3.0)*distance*ZOOM*sin(n);
        position.v[1] = MidY + pow(n, 1.0/3.0)*distance*ZOOM*cos(n);
        Vector velocity;
        double speedRange =  0.01;
        velocity.v[0] = -0.5*speedRange+rnd(speedRange);
        velocity.v[1] = - 0.5 * speedRange+rnd(speedRange);
        PManager->InitParticle(n, position, velocity, MASS, positiveOrNegative * CHARGE, RADIUS);
        const Particle& p = PManager->P(n);
        RedrawParticleAtNewPosition(n, p.Position.Get(0), p.Position.Get(1), p.Position.Get(0), p.Position.Get(1), p.Charge);
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
    UpdateParticlesForcesAndVelocities();
    HandleKeyPress();
#if CHECK_COLLISIONS
    AvoidCollisions();
#endif
    UpdateParticlesPositionsAndDraw();
    W.DrawScreen();
}

void Particles::UpdateParticlesForcesAndVelocities()
{
    for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
    {
        for (int n2 = n1-1; n2 >= 0; --n2) // this has to be done in this order, not "n2 = 0; n2 < n1;" because otherwise you cannot update speed in the same loop
        {
            ApplyForce(n1,n2);
        }
        PManager->UpdateVelocity(n1, BorderDimensions, DISSIPATION);
    }
}

void Particles::UpdateParticlesPositionsAndDraw()
{
    int oldX;
    int oldY;
    for(int n1 = PManager->PCount() - 1; n1 >= 0; --n1){
        oldX = PManager->P(n1).Position.Get(0);
        oldY = PManager->P(n1).Position.Get(1);
        PManager->UpdatePosition(n1);
        RedrawParticleAtNewPosition(n1, oldX, oldY, PManager->P(n1).Position.Get(0), PManager->P(n1).Position.Get(1), PManager->P(n1).Charge);
    }
}

void Particles::AvoidCollisions()
{
    for (int n1 = PManager->PCount() - 1; n1 >= 0; --n1)
    {
        for (int n2 = n1; n2 >= 0; --n2)
        {
            double distance = PManager->Distance(n1,n2);
            PManager->ResolveOverlapIfNeeded(n1,n2, distance);
            if (PManager->CheckCollisionImminent(n1, n2))
            {
                cout << "collision between " << n1 << " and " << n2 << endl;
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
    if (check =='r') {Init();  cout << "Reset" << endl;}
    if (check =='a') {RemoveParticle(PManager->P(PManager->PCount() - 1).Position.Get(0), PManager->P(PManager->PCount() - 1).Position.Get(1)); cout << "Remove particle" << endl;}
    if (check =='-') {GravitationalConstant /= 2; if (GravitationalConstant < 0.000000000001) GravitationalConstant = 0.0;  cout << "G =" << GravitationalConstant << endl;}
    if (check =='+') {GravitationalConstant *= 2; if (GravitationalConstant < 0.000000000001) GravitationalConstant = 0.00000000001;  cout << "G =" << GravitationalConstant << endl;}
    if (check =='1') {ElectrostaticConstant /= 2; if (ElectrostaticConstant < 0.000000000001) ElectrostaticConstant = 0.0;  cout << "E =" << ElectrostaticConstant << endl;}
    if (check =='2') {ElectrostaticConstant *= 2; if (ElectrostaticConstant < 0.000000000001) ElectrostaticConstant = 0.00000000001;  cout << "E =" << ElectrostaticConstant << endl;}
    if (check =='4') {MolecularBondingEnergy /= 2; if (MolecularBondingEnergy < 0.000000000001) MolecularBondingEnergy = 0.0;  cout << "Mol =" << MolecularBondingEnergy << endl;}
    if (check =='5') {MolecularBondingEnergy *= 2; if (MolecularBondingEnergy < 0.000000000001) MolecularBondingEnergy = 0.00000000001;  cout << "Mol =" << MolecularBondingEnergy << endl;}

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
