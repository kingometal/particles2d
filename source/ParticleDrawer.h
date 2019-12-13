#ifndef PARTICLEDRAWER_H
#define PARTICLEDRAWER_H

#include <chrono>

class ParticleManager;
class IParticlesView;
class Config;

class ParticleDrawer
{
public:
    ParticleDrawer() = default;
    ~ParticleDrawer() = default;

    void Draw(IParticlesView& view, const ParticleManager& pm, Config& params);

private:
    void DrawParticle(IParticlesView& view, const ParticleManager& pm, Config& params, int index) const;

    void DrawVelocity(IParticlesView& view, const ParticleManager& pm, Config& params, int index) const;

    std::chrono::microseconds LastDrawTime;
    std::chrono::microseconds LastFPSOutputTime;
    double MeanFps;
    double FPSValuesSum;
    double FPSValuesCount;
    double EKin;
    double EGrav;
    double EEl;
    double EMol;
    double ESum;
};

#endif // PARTICLEDRAWER_H
