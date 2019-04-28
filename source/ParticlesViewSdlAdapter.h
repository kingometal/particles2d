#ifndef PARTICLESVIEWSDLADAPTER_H
#define PARTICLESVIEWSDLADAPTER_H
#include "interfaces/ParticlesViewInterface.h"

class IPresenter;

class ParticlesViewSdlAdapter: public ParticlesViewInterface
{
public:
    ParticlesViewSdlAdapter(IPresenter& presenter, int sideX, int sideY);

    // ParticlesViewInterface interface
    void DrawParticle(int index, EColor color, int x, int y);
    void DrawParticle(EColor color, int x, int y);
    void DrawScreen();
    int GetSideX();
    int GetSideY();
    void ClearWindow();
private:
    IPresenter& Presenter;
    int SideX;
    int SideY;
};

#endif // PARTICLESVIEWSDLADAPTER_H
