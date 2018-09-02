#ifndef PARTICLE_H
#define PARTICLE_H

class Particle{
private:
	double x[2];
	double v[2];
	double prevx[2];

	double m;
    double q;
	double r;

public:
	double g[2];
        Particle();


        void stepV();
        void stepV(double L);
        void stepX(double L);
        void stepX();
        void decG(double addGx, double addGy);
        void incG(double addGx, double addGy);
        void setG(double newGx, double newGy);
        void setV(double newVx, double newVy);
        double getV2();
        double getVy();
        double getVx();
        void setPrev();
        void setX(double newX, double newY);
        double getPrevY();
        double getPrevX();
        double getY();
        double getX();
        void setQ(double newQ);
        double getQ();
        void setM(double newM);
        double getM();
        double getR();
        void setR(double newR);
};
#endif
