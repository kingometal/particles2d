#include "particles.h"
#include "particle.h"
#include "constants.h"
#include "include/rnd.h"
#include <iostream>

#include <cmath>
#include "particlesviewinterface.h"


Particles::Particles(ParticlesViewInterface &window):
    W(window),
    L(window.GetSide()*ZOOM),
    N(PARTICLE_COUNT),
    G(GRAVITY),
    E(ELECTRO),
    M(MAGNETI),
    Mol(ATOMIC),
    t(0)
{
    p = new Particle[N];
}

Particles::~Particles()
{
    delete[] p;
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


double Particles::oneDcollision(double v, double v2, double m, double m2) {
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
{   //Abstand zwischen Partikeln bestimmen
    return sqrt( (p[n2].getX()-p[n1].getX())*(p[n2].getX()-p[n1].getX())+(p[n2].getY()-p[n1].getY())*(p[n2].getY()-p[n1].getY())   )   ;
}

bool Particles::checkOverlap(int n1, int n2, double distance) {
    return distance < p[n1].getR()+p[n2].getR();
}

bool Particles::checkCollision(int n1, int n2) {
    return sqrt(pow( (p[n2].getX()+p[n2].getVx()) - (p[n1].getX()+p[n1].getVx()), 2.0 ) + pow( (p[n2].getY()+p[n2].getVy())-(p[n1].getY()+p[n1].getVy()),2.0)) < p[n1].getR()+p[n2].getR();
}

void Particles::overlap(int n1, int n2, double r) {
    if (checkOverlap(n1, n2, r)) {
        cout << "overlap" << endl;

        double x1 = p[n1].getX();
        double x2 = p[n2].getX();
        double y1 = p[n1].getY();
        double y2 = p[n2].getY();

        double rx = x2 - x1;
        double ry = y2 - y1;
        double cm = (p[n2].getM()*r)/(p[n2].getM()+p[n1].getM());  //center of mass
        double cmx = x1 + rx*cm/r;
        double cmy = y1 + ry*cm/r;

        double R = p[n1].getR() + p[n2].getR();
        double quot = R/r;
        p[n2].setX((x2-cmx)*quot + cmx,
                   (y2-cmy)*quot + cmy);
        p[n1].setX((x1-cmx)*quot + cmx,
                   (y1-cmy)*quot + cmy);
    }
}

void Particles::collision (int n1, int n2) {

    double tanr = (p[n1].getY()-p[n2].getY())/(p[n1].getX() - p[n2].getX());
    double cosr = 1/sqrt(tanr*tanr + 1.0);
    double sinr = tanr*cosr;

    if (p[n1].getX() - p[n2].getX() == 0.0) {
        tanr = 0.0;
        cosr = 0.0;
        sinr = 1.0;
    }

    double vGedrehtx1 = cosr*p[n1].getVx() + sinr*p[n1].getVy();
    double vGedrehty1 = cosr*p[n1].getVy() - sinr*p[n1].getVx();
    double vGedrehtx2 = cosr*p[n2].getVx() + sinr*p[n2].getVy();
    double vGedrehty2 = cosr*p[n2].getVy() - sinr*p[n2].getVx();

    double vGedrehttemp = vGedrehtx1;
    vGedrehtx1 = oneDcollision (vGedrehtx1, vGedrehtx2, p[n1].getM(), p[n2].getM());
    vGedrehtx2 = oneDcollision (vGedrehtx2, vGedrehttemp, p[n2].getM(), p[n1].getM());

    vGedrehttemp = vGedrehtx1;
    vGedrehtx1 = cosr*vGedrehtx1 - sinr*vGedrehty1;
    vGedrehty1 = cosr*vGedrehty1 + sinr*vGedrehttemp;
    vGedrehttemp = vGedrehtx2;
    vGedrehtx2 = cosr*vGedrehtx2 - sinr*vGedrehty2;
    vGedrehty2 = cosr*vGedrehty2 + sinr*vGedrehttemp;

    p[n1].setV(vGedrehtx1, vGedrehty1);
    p[n2].setV(vGedrehtx2, vGedrehty2);
}

void Particles::force(int n1, int n2, double r) {
    double fgx=0.0, fgy=0.0, fcx = 0.0, fcy = 0.0, fmx = 0.0, fmy = 0.0;

    double reziR3 = 1/(r*r*r);
    double reziR = 1.0/r;
    double x1 = p[n1].getX();
    double y1 = p[n1].getY();
    double q1 = p[n1].getQ();

    double x2 = p[n2].getX();
    double y2 = p[n2].getY();
    double q2 = p[n2].getQ();

    double dx = x1-x2;
    double dy = y1-y2;

    double lorentz = p[n2].getVx()*dy - p[n2].getVy()*dx;

    //Gravitationskraft
    {
        double pot = - G*p[n1].getM()*p[n2].getM()*reziR3;
        fgx += pot*dx;
        fgy += pot*dy;
    }

    //Coulombkraft und Lorenzkraft
    {
        double pot = q1*q2*reziR3;
        fcx = pot * (E*dx + M * p[n1].getVy()*lorentz );
        fcy = pot * (E*dy - M * p[n1].getVx()*lorentz );
    }

    //        //Molekulare Abstossende Kraft
//    if (r < ATOMIC_RADIUS * 20)
    {
        double pot = Mol * (pow(ATOMIC_RADIUS/r, 12) - pow(ATOMIC_RADIUS/r, 6)) ;
        fmx = pot * dx * reziR;
        fmy = pot * dy * reziR;
//        if (n1 == 0 && n2 == 1)
//        {
//            cout << "distance : " << r << " pot: " << pot << " = " << Mol << " * (" <<  pow(ATOMIC_RADIUS/r, 12) << " - "  <<  pow(ATOMIC_RADIUS/r, 6) <<  " )"   <<  "G: " << - G*p[n1].getM()*p[n2].getM()*reziR3*r <<  endl ;
//        }
    }

    p[n1].incG( fgx+fcx+fmx,fgy+fcy+fmy);
    p[n2].decG( fgx+fcx+fmx,fgy+fcy+fmy);
}

double Particles::eKin() {
    double En = 0.0;
    for (int n1 = 0; n1 < N; n1++) {
        En += 0.5*p[n1].getM()*p[n1].getV2();
    }
    return(En);
}

double Particles::eEl() {
    double En = 0.0;
    for (int n1 = 0; n1 < N; n1++) {
        for(int n2=n1+1; n2<N; n2++) {
            En+=E*p[n1].getQ()*p[n2].getQ()/getR(n1,n2);
        }
    }
    return(En);
}

double Particles::eG() {
    double En = 0.0;
    for (int n1 = 0; n1 < N; n1++) {
        for(int n2=n1+1; n2<N; n2++) {
            En=G*p[n1].getM()*p[n2].getM()/getR(n1,n2);
        }
    }
    return(En);
}

double Particles::eMol() {
    double En = 0.0;
    for (int n1 = 0; n1 < N; n1++) {
        for(int n2=n1+1; n2<N; n2++) {
            double r = getR(n1,n2);
            En-= 4*Mol * (pow(ATOMIC_RADIUS/r, 12) - pow(ATOMIC_RADIUS/r, 6)) ;
        }
    }
    return(En);
}


void Particles::redraw(int index, double oldX, double oldY, double x, double y, double q)
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

void Particles::redraw(int index)
{
    redraw(index, p[index].getPrevX(), p[index].getPrevY(), p[index].getX(), p[index].getY(), p[index].getQ());
}

void Particles::redrawV() {
//    W.Black();
//    W.DrawLine (x, y, x+vx*100, y+vy*100);
}


void Particles::redrawV(int color) {
//    W.Black();
//    if (color ==0) W.White();
//    W.DrawLine (x, y, x+vx*100, y+vy*100);
//    W.Black();
}



void Particles::sleeps( clock_t wait ) {
    clock_t goal;
    goal = wait + clock();
    while( goal > clock() );
}

void Particles::init(void) {
    InitRandom();
    double R =  RADIUS;
    int Mid = L/2;
    for (int n = 0; n<N; n++) {
        int pot = n%2 == 0 ? -1 : 1;
        //while (R<=1) R = rnd(5);
        double distance = 30.0;
        p[n].setX(Mid+pow(n, 1.0/3.0)*distance*ZOOM*sin(n), Mid+pow(n, 1.0/3.0)*distance*ZOOM*cos(n));
        p[n].setM(MASS);
        p[n].setQ(pot * CHARGE);
        double speedRange =  0.01;
        p[n].setV(-0.5*speedRange+rnd(speedRange), - 0.5 * speedRange+rnd(speedRange));//0.323478
        p[n].setG(0.0, 0.0);
        p[n].setR(R);
        cout << "n: " <<  n << " M:" << p[n].getM() << " Q:" << p[n].getQ() << " R:" << p[n].getR()<< endl;

        redraw(n, p[n].getX(), p[n].getY(), p[n].getX(), p[n].getY(), p[n].getQ());
        p[n].setPrev();
    }
}


bool Particles::update() {
    for (int n1=0; n1<N; n1++)
    {
        p[n1].setG(0.0, 0.0);
    }

    for (int n1=0; n1<N; n1++)
    {
        for (int n2=n1; n2<N; n2++)
        {
            if (n2!=n1)
            {
                double distance = getR(n1, n2);
                force(n1,n2,distance);
            }
        }
        p[n1].stepV(L);
    }
#if CHECK_COLLISIONS
    for (int n1 = 0; n1<N; n1++)
    {
        for (int n2 = 0; n2<n1; n2++)
        {
            double distance = getR(n1, n2);
            if (checkCollision(n1, n2))
            {
                cout << "collision" << n1 << " " << n2 << endl;
                collision(n1, n2);
                p[n1].setG(0.0,0.0);
                p[n2].setG(0.0,0.0);
                force (n1, n2, distance);
                p[n1].stepV(L);
                p[n2].stepV(L);
            }
            overlap(n1,n2, distance);
        }
    }
#endif

    t++;

    char check = W.CheckKeyPress();
    if (check != '0')
    {
        cout << check << endl;
    }
    if (check =='-') {
        G /= 2;
        if (G < 0.000000000001) G = 0.0;
        cout << "G =" << G << endl;
    }
    if (check =='+') {
        G *= 2;
        if (G < 0.000000000001) G = 0.00000000001;
        cout << "G =" << G << endl;
    }
    if (check =='1') {
        E /= 2;
        if (E < 0.000000000001) E = 0.0;
        cout << "E =" << E << endl;
    }
    if (check =='2') {
        E *= 2;
        if (E < 0.000000000001) E = 0.00000000001;
        cout << "E =" << E << endl;
    }
    if (check =='4') {
        Mol /= 2;
        if (Mol < 0.000000000001) Mol = 0.0;
        cout << "Mol =" << Mol << endl;
    }
    if (check =='5') {
        Mol *= 2;
        if (Mol < 0.000000000001) Mol = 0.00000000001;
        cout << "Mol =" << Mol << endl;
    }

    if (check =='/') {
        for (int i = 0; i < N; i++) {
            if (p[i].getR()>0.1) {
                p[i].setR(p[i].getR()-0.1);
            }

        }
        cout << "radius reduced by 0.1; it is now " << p[0].getR() << endl;
    }
    if (check =='*') {
        for (int i = 0; i < N; i++) {
            p[i].setR(p[i].getR()+0.1);

        }
        cout << "radius increased by 0.1; it is now " << p[0].getR() << endl;
    }
    if (check =='q') {
        return false;
    }


    for(int n1=0; n1<N; n1++) {
        p[n1].stepX();
        redraw(n1);
        p[n1].setPrev();
    }
    return true;
}
