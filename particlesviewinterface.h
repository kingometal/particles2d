#ifndef PARTICLESVIEWINTERFACE_H
#define PARTICLESVIEWINTERFACE_H


enum EColor
{
    Black = 0,
    White,
    Red,
    Blue
};

class ParticlesViewInterface
{
public:
    ParticlesViewInterface() {}
    virtual ~ParticlesViewInterface() {}

    virtual void DrawParticle(int index, EColor color, int x, int y) = 0;
    virtual char CheckKeyPress() = 0;
    virtual int GetSide() = 0;

};

#endif // PARTICLESVIEWINTERFACE_H

