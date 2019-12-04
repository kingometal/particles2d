#ifndef PARTICLESVIEWSDLADAPTER_H
#define PARTICLESVIEWSDLADAPTER_H
#include "interfaces/ParticlesViewInterface.h"

class IPresenter;

class ParticlesViewSdlAdapter: public ParticlesViewInterface
{
public:
    ParticlesViewSdlAdapter(IPresenter& presenter, int sideX, int sideY, int maxFPS);

    // ParticlesViewInterface interface
    void DrawParticle(int index, RGBData color, int x, int y);
    void DrawParticle(RGBData color, int x, int y);
    void DrawScreen();
    int GetSideX();
    int GetSideY();
    void ClearWindow(RGBData color);
private:
    IPresenter& Presenter;
    int SideX;
    int SideY;
    int MaxFPS;
};

#endif // PARTICLESVIEWSDLADAPTER_H