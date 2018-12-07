#ifndef PARTICLESVIEWSDLADAPTER_H
#define PARTICLESVIEWSDLADAPTER_H
#include "particlesviewinterface.h"

class IPresenter;

class particlesViewSdlAdapter: public ParticlesViewInterface
{
public:
    particlesViewSdlAdapter(IPresenter& presenter, int side);

    // ParticlesViewInterface interface
    void DrawParticle(int index, EColor color, int x, int y);
    void DrawParticle(EColor color, int x, int y);
    char CheckKeyPress();
    void DrawScreen();
    int GetSide();
private:
    IPresenter& Presenter;
    int Side;
};

#endif // PARTICLESVIEWSDLADAPTER_H
