#ifndef PARTICLESVIEWSDLADAPTER_H
#define PARTICLESVIEWSDLADAPTER_H
#include "interfaces/ParticlesViewInterface.h"

class IPresenter;

class ParticlesViewSdlAdapter: public ParticlesViewInterface
{
public:
    ParticlesViewSdlAdapter(IPresenter& presenter, int side);

    // ParticlesViewInterface interface
    void DrawParticle(int index, EColor color, int x, int y);
    void DrawParticle(EColor color, int x, int y);
    char CheckKeyPress();
    void DrawScreen();
    int GetSide();
    void ClearWindow();
private:
    IPresenter& Presenter;
    int Side;
};

#endif // PARTICLESVIEWSDLADAPTER_H
