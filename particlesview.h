#ifndef PARTICLESVIEW_H
#define PARTICLESVIEW_H

#include "particlesviewinterface.h"
#include "xwindows.h"
#include "plugin/IUserInput.h"

class ParticlesView: public ParticlesViewInterface, public XWindow, public IUserInput
{
public:
    ParticlesView(int side);
    virtual ~ParticlesView();

    // ParticlesViewInterface interface
    void DrawParticle(int index, EColor color, int x, int y);
    void DrawParticle(EColor color, int x, int y);
    char CheckKeyPress();
    bool IsQuitRequested();
    int GetSide();
    void DrawScreen();

private:
    void SetColor(EColor color);
    int Side;
};

#endif // PARTICLESVIEW_H
