#include "particles.h"
#include "particle.h"
#include "constants.h"
#include "include/rnd.h"
#include <iostream>
#include "plugin/IUserInput.h"
#include <cmath>
#include "particlesviewinterface.h"


Particles::Particles(ParticlesViewInterface &window, IUserInput & userInput):
    W(window),
    UserInput (userInput),
    L(window.GetSide()*ZOOM),
    NumParticles(PARTICLE_COUNT),
    GravitationalConstant(GRAVITY),
    ElectrostaticConstant(ELECTRO),
    MagneticPermeability(MAGNETI),
    MolecularBondingEnergy(ATOMIC)
{
    ParticleN = new Particle[NumParticles];
}

Particles::~Particles()
{
    delete[] ParticleN;
}


//void Particles::get(int n){
//    x = p[n].getX();
//    y = p[n].getY();
//    R = p[n].getR();
//    q = p[n].getQ();
//    vx = p[n].getVx();
//    vy = p[n].getVy();
//}

//void Particles::getPrev(int n){
//    x = p[n].getPrevX();
//    y = p[n].getPrevY();
//}


double Particles::oneDcollision(double v, double v2, double m, double m2){
    return (2.0 * m2 * v2   + (m - m2)* v) / (m + m2);
}

//void Particles::getV(int n){
//    x = L/2 + p[n].getVx()*30;
//    y = L/2 + p[n].getVy()*30;
//}

//void Particles::getV2(int n){
//    x = L/2+n;
//    y = L/2 - (p[n].getVx()*p[n].getVx()+p[n].getVy()*p[n].getVy())*30;
//}

double Particles::getR (int n1, int n2)
{ //Abstand zwischen Partikeln bestimmen
    return sqrt( pow(ParticleN[n2].GetXPosition()-ParticleN[n1].GetXPosition(),2)+pow(ParticleN[n2].GetYPosition()-ParticleN[n1].GetYPosition(),2));
}

bool Particles::CheckOverlap(int n1, int n2, double distance){
    return distance < ParticleN[n1].GetRadius()+ParticleN[n2].GetRadius();
}

bool Particles::CheckCollisionImminent(int n1, int n2){
    return sqrt(pow( (ParticleN[n2].GetXPosition()+ParticleN[n2].GetXVelocity()) - (ParticleN[n1].GetXPosition()+ParticleN[n1].GetXVelocity()) , 2.0 ) + pow( (ParticleN[n2].GetYPosition()+ParticleN[n2].GetYVelocity())-(ParticleN[n1].GetYPosition()+ParticleN[n1].GetYVelocity()) ,2.0)) < ParticleN[n1].GetRadius()+ParticleN[n2].GetRadius();
}

void Particles::ResolveOverlapIfNeeded(int n1, int n2, double r){
    if (CheckOverlap(n1, n2, r)){
        //        cout << "overlap" << endl;

        double x1 = ParticleN[n1].GetXPosition();
        double x2 = ParticleN[n2].GetXPosition();
        double y1 = ParticleN[n1].GetYPosition();
        double y2 = ParticleN[n2].GetYPosition();

        double rx = x2 - x1;
        double ry = y2 - y1;
        double cm = (ParticleN[n2].GetMass()*r)/(ParticleN[n2].GetMass()+ParticleN[n1].GetMass());  //center of mass
        double cmx = x1 + rx*cm/r;
        double cmy = y1 + ry*cm/r;

        double R = ParticleN[n1].GetRadius() + ParticleN[n2].GetRadius();
        double quot = R/r;
        ParticleN[n2].SetPosition((x2-cmx)*quot + cmx, (y2-cmy)*quot + cmy);
        ParticleN[n1].SetPosition((x1-cmx)*quot + cmx, (y1-cmy)*quot + cmy);
    }
}

void Particles::PerformCollision (int n1, int n2){

    double tanr = (ParticleN[n1].GetYPosition()-ParticleN[n2].GetYPosition())/(ParticleN[n1].GetXPosition() - ParticleN[n2].GetXPosition());
    double cosr = 1/sqrt(tanr*tanr + 1.0);
    double sinr = tanr*cosr;

    if (ParticleN[n1].GetXPosition() - ParticleN[n2].GetXPosition() == 0.0) {
        tanr = 0.0;
        cosr = 0.0;
        sinr = 1.0;
    }

    double vGedrehtx1 = cosr*ParticleN[n1].GetXVelocity() + sinr*ParticleN[n1].GetYVelocity();
    double vGedrehty1 = cosr*ParticleN[n1].GetYVelocity() - sinr*ParticleN[n1].GetXVelocity();
    double vGedrehtx2 = cosr*ParticleN[n2].GetXVelocity() + sinr*ParticleN[n2].GetYVelocity();
    double vGedrehty2 = cosr*ParticleN[n2].GetYVelocity() - sinr*ParticleN[n2].GetXVelocity();

    double vGedrehttemp = vGedrehtx1;
    vGedrehtx1 = oneDcollision (vGedrehtx1, vGedrehtx2, ParticleN[n1].GetMass(), ParticleN[n2].GetMass());
    vGedrehtx2 = oneDcollision (vGedrehtx2, vGedrehttemp, ParticleN[n2].GetMass(), ParticleN[n1].GetMass());

    vGedrehttemp = vGedrehtx1;
    vGedrehtx1 = cosr*vGedrehtx1 - sinr*vGedrehty1;
    vGedrehty1 = cosr*vGedrehty1 + sinr*vGedrehttemp;
    vGedrehttemp = vGedrehtx2;
    vGedrehtx2 = cosr*vGedrehtx2 - sinr*vGedrehty2;
    vGedrehty2 = cosr*vGedrehty2 + sinr*vGedrehttemp;

    ParticleN[n1].SetVelocity(vGedrehtx1, vGedrehty1);
    ParticleN[n2].SetVelocity(vGedrehtx2, vGedrehty2);
}

double Particles::Mass(int n)
{
    return ParticleN[n].GetMass();
}

double Particles::Charge(int n)
{
    return ParticleN[n].GetCharge();
}

void Particles::ApplyForce(int n1, int n2){
    double fgx=0.0, fgy=0.0, fcx = 0.0, fcy = 0.0, fmx = 0.0, fmy = 0.0;
    double dx = ParticleN[n1].GetXPosition()-ParticleN[n2].GetXPosition();
    double dy = ParticleN[n1].GetYPosition()-ParticleN[n2].GetYPosition();
    double distance = hypot(dx, dy);
    double reziR = 1.0/distance;
    double reziR3 = pow(reziR, 3);


    //Gravitationskraft
    {
        double pot = - GravitationalConstant*Mass(n1)*Mass(n2)*reziR3;
        fgx = pot*dx;
        fgy = pot*dy;
    }

    //Coulombkraft und Lorenzkraft
    {
        double pot = Charge(n1)*Charge(n2)*reziR3;
        double lorentz = ParticleN[n2].GetXVelocity()*dy - ParticleN[n2].GetYVelocity()*dx;
        fcx = pot * (ElectrostaticConstant*dx + MagneticPermeability * ParticleN[n1].GetYVelocity()*lorentz );
        fcy = pot * (ElectrostaticConstant*dy - MagneticPermeability * ParticleN[n1].GetXVelocity()*lorentz );
    }

    // Lennard-Jones Potential Force
    if (distance < ATOMIC_RADIUS * 20)
    {
        double arr = pow(ATOMIC_RADIUS*reziR , 6);
        double pot = MolecularBondingEnergy * (pow(arr, 2) - arr) * reziR ;
        fmx = pot * dx;
        fmy = pot * dy;
    }

    ParticleN[n1].IncreaseForce( fgx+fcx+fmx,fgy+fcy+fmy);
    ParticleN[n2].DecreaseForce( fgx+fcx+fmx,fgy+fcy+fmy);
}

double Particles::eKin(){
    double En = 0.0;
    for (int n1 = 0; n1 < NumParticles; n1++){
        En += 0.5*ParticleN[n1].GetMass()*ParticleN[n1].GetSquaredVelocity();
    }
    return(En);
}

double Particles::eEl(){
    double En = 0.0;
    for (int n1 = 0; n1 < NumParticles; n1++){
        for(int n2=n1+1; n2<NumParticles; n2++){
            En+=ElectrostaticConstant*ParticleN[n1].GetCharge()*ParticleN[n2].GetCharge()/getR(n1,n2);
        }
    }
    return(En);
}

double Particles::eG(){
    double En = 0.0;
    for (int n1 = 0; n1 < NumParticles; n1++){
        for(int n2=n1+1; n2<NumParticles; n2++){
            En=GravitationalConstant*ParticleN[n1].GetMass()*ParticleN[n2].GetMass()/getR(n1,n2);
        }
    }
    return(En);
}

double Particles::eMol(){
    double En = 0.0;
    for (int n1 = 0; n1 < NumParticles; n1++){
        for(int n2=n1+1; n2<NumParticles; n2++){
            double r = getR(n1,n2);
            En-= 4*MolecularBondingEnergy * (pow(ATOMIC_RADIUS/r, 12) - pow(ATOMIC_RADIUS/r, 6)) ;
        }
    }
    return(En);
}


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

void Particles::RedrawParticleAtNewPosition(int index)
{
    RedrawParticleAtNewPosition(index, ParticleN[index].GetSavedXPosition(), ParticleN[index].GetSavedYPosition(), ParticleN[index].GetXPosition(), ParticleN[index].GetYPosition(), Charge(index));
}

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

void Particles::sleeps( clock_t wait ){
    clock_t goal;
    goal = wait + clock();
    while( goal > clock() );
}

void Particles::Init(void){
    InitRandom();
    W.ClearWindow();
    double R =  RADIUS;
    int Mid = L/2;
    for (int n = 0; n<NumParticles; n++){
        int positiveOrNegative = n%2 == 0 ? -1 : 1;
        double distance = 30.0;
        ParticleN[n].SetPosition(Mid+pow(n, 1.0/3.0)*distance*ZOOM*sin(n), Mid+pow(n, 1.0/3.0)*distance*ZOOM*cos(n));
        ParticleN[n].SavePosition();
        ParticleN[n].SetMass(MASS);
        ParticleN[n].SetCharge(positiveOrNegative * CHARGE);
        double speedRange =  0.01;
        ParticleN[n].SetVelocity(-0.5*speedRange+rnd(speedRange), - 0.5 * speedRange+rnd(speedRange));//0.323478
        ParticleN[n].SetForce(0.0, 0.0);
        ParticleN[n].SetRadius(R);
        cout << "n: " <<  n << " M:" << ParticleN[n].GetMass() << " Q:" << ParticleN[n].GetCharge() << " R:" << ParticleN[n].GetRadius()<< endl;

        RedrawParticleAtNewPosition(n, ParticleN[n].GetXPosition(), ParticleN[n].GetYPosition(), ParticleN[n].GetXPosition(), ParticleN[n].GetYPosition(), ParticleN[n].GetCharge());
    }
    W.DrawScreen();
}

void Particles::AddParticle(int x, int y, double dx, double dy)
{
    Particle* tempParticlesN = new Particle[NumParticles + 1];
    for (int i = 0; i < NumParticles; i++)
    {
        tempParticlesN[i] = ParticleN[i];
    }
    tempParticlesN[NumParticles].SetCharge(0);
    tempParticlesN[NumParticles].SetForce(0,0);
    tempParticlesN[NumParticles].SetMass(MASS);
    tempParticlesN[NumParticles].SetPosition(x,y);
    tempParticlesN[NumParticles].SetRadius(RADIUS);
    tempParticlesN[NumParticles].SetVelocity(dx/500.0,dy/500.0);
    delete [] ParticleN;
    ParticleN = tempParticlesN;
    NumParticles++;
}

int Particles::GetClosestParticle(int x, int y)
{
    int closestParticle = -1;
    double shortestDistance = MAXFLOAT;
    for (int i = 0; i < NumParticles; i++)
    {
        double distance = sqrt( pow(x-ParticleN[i].GetXPosition(),2)+pow(y-ParticleN[i].GetYPosition(),2));
        if (distance < shortestDistance)
        {
            closestParticle = i;
            shortestDistance = distance;
        }
    }
    return closestParticle;
}

void Particles::RemoveParticle(int index)
{
    Particle* tempParticlesN = new Particle[NumParticles - 1];
    int j = 0;
    for (int i = 0; i < NumParticles-1; i++)
    {
        if (i == index)
        {
            j++;
        }
        tempParticlesN[i] = ParticleN[j];
        j++;

    }
    delete [] ParticleN;
    ParticleN = tempParticlesN;
    NumParticles--;
}

bool Particles::RemoveParticle(int x, int y)
{
    bool removed = false;
    if (NumParticles > 0)
    {
        int closestParticle = GetClosestParticle(x,y);
        double distance = sqrt( pow(x-ParticleN[closestParticle].GetXPosition(),2)+pow(y-ParticleN[closestParticle].GetYPosition(),2));
        if (distance < ATOMIC_RADIUS)
        {
            W.DrawParticle(closestParticle, White, ParticleN[closestParticle].GetSavedXPosition(), ParticleN[closestParticle].GetSavedYPosition());
            RemoveParticle(closestParticle);
            W.ClearWindow();
            removed = true;
            cout << "removed" << endl;
        }
    }
    return removed;
}


void Particles::Update(){
    ResetParticlesForces();
    UpdateParticlesForcesAndVelocities();
    HandleKeyPress();
#if CHECK_COLLISIONS
    AvoidCollisions();
#endif
    UpdateParticlesPositionsAndDraw();
    W.DrawScreen();
}

void Particles::ResetParticlesForces()
{
    for (int n=0; n<NumParticles; n++)
    {
        ParticleN[n].SetForce(0.0, 0.0);
    }
}

void Particles::UpdateParticlesForcesAndVelocities()
{
    for (int n1=0; n1<NumParticles; n1++)
    {
        for (int n2=n1+1; n2<NumParticles; n2++) // this has to be done in this order, not "n2 = 0; n2 < n1;" because otherwise you cannot update speed in the same loop
        {
            if (n2!=n1)
            {
                ApplyForce(n1,n2);
            }
        }
        ParticleN[n1].UpdateSpeedAccordingToForceDissipationAndBorders(L);
    }
}

void Particles::UpdateParticlesPositionsAndDraw()
{
    for(int n1=0; n1<NumParticles;n1++){
        ParticleN[n1].MoveAccordingToCurrentVelocity();
        RedrawParticleAtNewPosition(n1);
        ParticleN[n1].SavePosition();
    }
}

void Particles::AvoidCollisions()
{
    for (int n1 = 0; n1<NumParticles; n1++)
    {
        for (int n2 = 0; n2<n1; n2++)
        {
            double distance = getR(n1, n2);
            ResolveOverlapIfNeeded(n1,n2, distance);
            if (CheckCollisionImminent(n1, n2))
            {
                PerformCollision(n1, n2);
                ParticleN[n1].SetForce(0.0,0.0);
                ParticleN[n2].SetForce(0.0,0.0);
                ApplyForce(n1, n2);
                ParticleN[n1].UpdateSpeedAccordingToForceDissipationAndBorders(L);
                ParticleN[n2].UpdateSpeedAccordingToForceDissipationAndBorders(L);
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
        cout << "click" << endl;
        if (! RemoveParticle(click.x*2, click.y*2))
        {
            AddParticle(click.x*2, click.y*2, click.dx, click.dy);
        }
    }
    if (check != 0 && check != '0')
    {
        cout << check << endl;
    }
    if (check =='r') {Init();  cout << "Reset" << endl;}
    if (check =='a') {RemoveParticle(ParticleN[NumParticles-1].GetXPosition(), ParticleN[NumParticles-1].GetYPosition());  cout << "Remove particle" << endl;}
    if (check =='-') {GravitationalConstant /= 2; if (GravitationalConstant < 0.000000000001) GravitationalConstant = 0.0;  cout << "G =" << GravitationalConstant << endl;}
    if (check =='+') {GravitationalConstant *= 2; if (GravitationalConstant < 0.000000000001) GravitationalConstant = 0.00000000001;  cout << "G =" << GravitationalConstant << endl;}
    if (check =='1') {ElectrostaticConstant /= 2; if (ElectrostaticConstant < 0.000000000001) ElectrostaticConstant = 0.0;  cout << "E =" << ElectrostaticConstant << endl;}
    if (check =='2') {ElectrostaticConstant *= 2; if (ElectrostaticConstant < 0.000000000001) ElectrostaticConstant = 0.00000000001;  cout << "E =" << ElectrostaticConstant << endl;}
    if (check =='4') {MolecularBondingEnergy /= 2; if (MolecularBondingEnergy < 0.000000000001) MolecularBondingEnergy = 0.0;  cout << "Mol =" << MolecularBondingEnergy << endl;}
    if (check =='5') {MolecularBondingEnergy *= 2; if (MolecularBondingEnergy < 0.000000000001) MolecularBondingEnergy = 0.00000000001;  cout << "Mol =" << MolecularBondingEnergy << endl;}

    if (check =='/') {
        for (int i = 0; i < NumParticles; i++){
            if (ParticleN[i].GetRadius()>0.1){
                ParticleN[i].SetRadius(ParticleN[i].GetRadius()-0.1);
            }

        }
        cout << "radius reduced by 0.1; it is now " << ParticleN[0].GetRadius() << endl;
    }
    if (check =='*') {
        for (int i = 0; i < NumParticles; i++){
            ParticleN[i].SetRadius(ParticleN[i].GetRadius()+0.1);

        }
        cout << "radius increased by 0.1; it is now " << ParticleN[0].GetRadius() << endl;
    }
}
