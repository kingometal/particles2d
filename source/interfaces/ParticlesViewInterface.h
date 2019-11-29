#ifndef PARTICLESVIEWINTERFACE_H
#define PARTICLESVIEWINTERFACE_H

#include "../RGBData.h"

class ParticlesViewInterface
{
public:
    ParticlesViewInterface() {}
    virtual ~ParticlesViewInterface() {}

    virtual void DrawParticle(int index, RGBData color, int x, int y) = 0;
    virtual void DrawParticle(RGBData color, int x, int y) = 0;
    virtual void DrawScreen() = 0;
    virtual int GetSideX() = 0;
    virtual int GetSideY() = 0;
    virtual void ClearWindow(RGBData color) = 0;

};

#endif // PARTICLESVIEWINTERFACE_H

