#ifndef I_PARTICLES_VIEW_H
#define I_PARTICLES_VIEW_H

#include "../RGBData.h"

class IParticlesView
{
public:
    IParticlesView() {}
    virtual ~IParticlesView() {}

    virtual void DrawParticle(int index, RGBData color, int x, int y) = 0;
    virtual void DrawParticle(RGBData color, int x, int y) = 0;
    virtual void DrawLine(RGBData color, int x, int y, int dx, int dy) = 0;
    virtual void DrawText(const char* text, const RGBData &color, int x, int y) = 0;
    virtual void DrawScreen() = 0;
    virtual int GetSideX() = 0;
    virtual int GetSideY() = 0;
    virtual void ClearWindow(RGBData color) = 0;

};

#endif // I_PARTICLES_VIEW_H

