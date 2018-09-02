#include "particles.h"
#include "constants.h"
#include "particlesview.h"
#include <iostream>

int main(void){
    double en;
    ParticlesView* W = new ParticlesView(WINDOW_SIDE);
    Particles Ps(*W);
    Ps.init();
    int j = 0;
    do
    {
        for (int i = 0; i<1; i++)
        {
            Ps.sleeps(1.0);
            if (!Ps.update())
            {
                delete W;
                return 0;
            }
        }
    }
    while (true);
}

