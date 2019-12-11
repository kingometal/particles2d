#ifndef PARTICLESVIEWSDLADAPTER_H
#define PARTICLESVIEWSDLADAPTER_H
#include "interfaces/IParticlesView.h"

class IPresenter;

class ParticlesViewSdlAdapter: public IParticlesView
{
public:
    ParticlesViewSdlAdapter(IPresenter& presenter, int sideX, int sideY, int maxFPS);

    // ParticlesViewInterface interface
    void DrawParticle(int index, RGBData color, int x, int y);
    void DrawParticle(RGBData color, int x, int y);
    void DrawLine(RGBData color, int x, int y, int dx, int dy);
    void DrawScreen();
    int GetSideX();
    int GetSideY();
    void ClearWindow(RGBData color);
private:
    IPresenter& Presenter;
    int MaxFPS;
};

#endif // PARTICLESVIEWSDLADAPTER_H
