#ifndef PARTICLES_H
#define PARTICLES_H

#include <ctime>


class Particle;
class ParticlesViewInterface;

class Particles{
public:
    Particles(ParticlesViewInterface& window);
    ~Particles();
    bool update();
    void init();

    double eEl();
    double eKin();
    double eMol();
    double eG();
    void sleeps(clock_t wait);

private:
    void redrawV(int color);
    void redrawV();
    void redraw(int index, double oldX, double oldY, double x, double y, double q);
    void redraw(int index);
    void force(int n1, int n2, double r);
    void collision(int n1, int n2);
    void overlap(int n1, int n2, double r);
    bool checkCollision(int n1, int n2);
    bool checkOverlap(int n1, int n2, double distance);
    double getR(int n1, int n2);
    double oneDcollision(double v, double v2, double m, double m2);

    const int L;
    ParticlesViewInterface &W;

//    double x;
//    double y;
//    double vx;
//    double vy;
//    double q;
//    double R;
//    double X;
//    double Y;

    const int N;
    Particle* p;
    double G;
    double E;
    const double M;
    double Mol;
    int t;
};

#endif


