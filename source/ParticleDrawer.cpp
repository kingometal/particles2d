#include "ParticleDrawer.h"
#include "interfaces/IParticlesView.h"
#include "ParticleManager.h"
#include "Config.h"
#include "Physics.h"
#include "Particle.h"
#include <cmath>
#include <string>


using namespace std;
using namespace std::chrono;

void ParticleDrawer::Draw(IParticlesView& view, const ParticleManager& pm, Config& params)
{
    microseconds timeNow = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    double fps = 1000000.0/(timeNow - LastDrawTime).count();

    if (0 == params.MaxFPS || fps < params.MaxFPS)
    {
        view.ClearWindow(params.BackgroundColor);

        FPSValuesSum += fps;
        ++FPSValuesCount;

        if ((timeNow - LastFPSOutputTime).count() > 1000000.0 )
        {
            MeanFps = FPSValuesSum / FPSValuesCount;
            LastFPSOutputTime = timeNow;
            FPSValuesCount = 0;
            FPSValuesSum = 0;
            EKin = Physics::eKin(&pm, params);
            EGrav = Physics::eG(&pm, params);
            EEl = Physics::eEl(&pm, params);
            EMol = Physics::eMol(&pm, params);
            ESum = EKin + EGrav + EEl + EMol;
        }

        view.DrawText(to_string(MeanFps).c_str(), params.UnchargedParticleColor, 0, 0);
        if (params.ShowEnergies)
        {
            char target [50];
            snprintf(target, 50, "EKin : %.16f", EKin);
            view.DrawText(target, params.UnchargedParticleColor, 0, 24);
            snprintf(target, 50, "EGrav: %.16f", EGrav);
            view.DrawText(target, params.UnchargedParticleColor, 0, 48);
            snprintf(target, 50, "EEl  : %.16f", EEl);
            view.DrawText(target, params.UnchargedParticleColor, 0, 72);

            snprintf(target, 50, "EMol : %.16f", EMol);
            view.DrawText(target, params.UnchargedParticleColor, 0, 96);

            snprintf(target, 50, "ETot : %.16f", ESum);
            view.DrawText(target, params.UnchargedParticleColor, 0, 120);
        }


        for (int i = 0; i < pm.PCount(); i++)
        {
            const Particle & p = pm.P(i);
            DrawParticle(view, pm, params, i);
            if (params.DrawVelocities)
            {
                DrawVelocity(view, pm, params, i);
            }
        }
        view.DrawScreen();
        LastDrawTime = timeNow;
    }
}

void ParticleDrawer::DrawParticle(IParticlesView& view, const ParticleManager& pm, Config& params, int index) const
{
    const Particle& p = pm.P(index);

    int x = p.Position.X()/params.Scale;
    int y = p.Position.Y()/params.Scale;
    RGBData particleColor = params.UnchargedParticleColor;

    if(p.Charge>0.0)
    {
        particleColor = params.PositivelyChargedParticleColor;
    }
    else if (p.Charge<0.0)
    {
        particleColor = params.NegativeChargedParticleColor;
    }
    view.DrawParticle(index,particleColor, x,y);
}

void ParticleDrawer::DrawVelocity(IParticlesView& view, const ParticleManager& pm, Config& params, int index) const
{
    const Particle& p = pm.P(index);
    double rezScale = 1.0 / (double) params.Scale;
    double factor = params.VelocityLengthFactor;
    view.DrawLine(params.VelocityColor, p.Position.X() * rezScale, p.Position.Y() * rezScale, p.Velocity.X()*factor, p.Velocity.Y()*factor);
}
