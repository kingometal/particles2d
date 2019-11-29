#ifndef PARTICLESVIEW_H
#define PARTICLESVIEW_H

#include "interfaces/ParticlesViewInterface.h"
#include "XWindows.h"
#include "interfaces/IUserInput.h"

class ParticlesView: public ParticlesViewInterface, public XWindow, public IUserInput
{
public:
    ParticlesView(int side);
    virtual ~ParticlesView();

    // ParticlesViewInterface interface
    void DrawParticle(int index, RGBData color, int x, int y);
    void DrawParticle(RGBData color, int x, int y);
    char CheckKeyPress();
    bool IsQuitRequested();
    int GetSideX();
    int GetSideY();
    void DrawScreen();
    MouseClick CheckMouseClick();
    void ClearWindow(RGBData color);

private:
    void SetColor(RGBData color);
    int Side;
};

#endif // PARTICLESVIEW_H
