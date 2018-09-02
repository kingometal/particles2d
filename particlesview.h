#ifndef PARTICLESVIEW_H
#define PARTICLESVIEW_H

#include "particlesviewinterface.h"
#include "xwindows.h"

class ParticlesView: public ParticlesViewInterface, public XWindow
{
public:
    ParticlesView(int side);
    virtual ~ParticlesView();

    // ParticlesViewInterface interface
    void DrawParticle(int index, EColor color, int x, int y);
    char CheckKeyPress();
    int GetSide();

private:
    void SetColor(EColor color);
    int Side;
};

#endif // PARTICLESVIEW_H
