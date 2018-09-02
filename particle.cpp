#include "particle.h"
#include "constants.h"


Particle::Particle(void){
        for (int n = 0; n< 2; n++){
                x[n] = 0;
                v[n] = 0;
                g[n] = 0;
        }
        m=MASS;
        q=CHARGE;
        r=RADIUS;
}


void  Particle::setR(double newR){
        r = newR;
}

double Particle::getR(){
        return r;
}

double Particle::getM(){
        return m;
}

void Particle::setM(double newM){
        m = newM;
}

double Particle::getQ(){
        return q;
}

void Particle::setQ(double newQ){
        q = newQ;
}

double Particle::getX(void){
        return x[0];
}

double Particle::getY(void){
        return x[1];
}

double Particle::getPrevX(void){
        return prevx[0];
}

double Particle::getPrevY(void){
        return prevx[1];
}



void Particle::setX(double newX, double newY){
        x[0] = newX;
        x[1] = newY;
}

void Particle::setPrev(){
        prevx[0] = x[0];
        prevx[1] = x[1];
}

double Particle::getVx(void){
        return v[0];
}

double Particle::getVy(void){
        return v[1];
}

double Particle::getV2(void){
        return v[0]*v[0]+v[1]*v[1];
}

void Particle::setV(double newVx, double newVy){
        v[0] = newVx;
        v[1] = newVy;
}

void Particle::setG(double newGx, double newGy){
        g[0] = newGx;
        g[1] = newGy;

}

void Particle::incG(double addGx, double addGy){
        g[0] += addGx;
        g[1] += addGy;
}

void Particle::decG(double addGx, double addGy){
        g[0] -= addGx;
        g[1] -= addGy;
}

void Particle::stepX(){
        x[0]+=v[0];
        x[1]+=v[1];
}

void Particle::stepX(double L){
        x[0]+=v[0];
        x[1]+=v[1];
}


void Particle::stepV(double L){
        for (int n=0; n<2 ;n++){
                if (x[n]+v[n]+g[n]/m >= L or x[n]+v[n]+g[n]/m <0) {v[n]+= g[n]/m; v[n] *= -1;}
                v[n] += g[n]/m;
                v[n] = v[n]*(1 - 0.01*DISSIPATION_PERCENT);
        }
}

void Particle::stepV(){
        for (int n=0; n<2 ;n++){
                v[n] += g[n]/m;
        }
}
