#include "Physics.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "Config.h"
#include <cmath>


namespace Physics
{
Vector GetGravity(const Particle& p1, const Particle& p2, double reziprocalDistance, double G)
{
    double dx = p1.Position.Get(0) - p2.Position.Get(0);
    double dy = p1.Position.Get(1) - p2.Position.Get(1);
    double pot = - G * p1.Mass * p2.Mass * reziprocalDistance * reziprocalDistance * reziprocalDistance;
    double fgx = pot * dx;
    double fgy = pot * dy;
    return Vector (fgx, fgy);
}

Vector GetElectromagneticForce(const Particle& p1, const Particle& p2, double reziprocalDistance, double E, double M)
{
    double dx = p1.Position.Get(0) - p2.Position.Get(0);
    double dy = p1.Position.Get(1) - p2.Position.Get(1);
    double pot = p1.Charge * p2.Charge * reziprocalDistance * reziprocalDistance * reziprocalDistance;
    double lorentz = p2.Velocity.Get(0) * dy - p2.Velocity.Get(1) * dx;
    double fcx = pot * (E * dx + M * p1.Velocity.Get(1) * lorentz );
    double fcy = pot * (E * dy - M * p1.Velocity.Get(0) * lorentz );
    return Vector (fcx, fcy);
}

Vector GetLennardJonesForce(const Particle& p1, const Particle& p2, double distance, double reziprocalDistance, double atomicRadius, double bondingEnergy)
{
    double fmx = 0.0;
    double fmy = 0.0;
    if (bondingEnergy != 0 && distance < atomicRadius * 10)
    {
        double dx = p1.Position.Get(0) - p2.Position.Get(0);
        double dy = p1.Position.Get(1) - p2.Position.Get(1);
        double sigma6 = pow (atomicRadius, 6);
        double rMinus13 = pow (reziprocalDistance, 13);
        double rMinus7 = pow (reziprocalDistance, 7);
        double pot = 4 * bondingEnergy * ( 12 * pow(sigma6, 2) * rMinus13 - 6 * sigma6 * rMinus7) * reziprocalDistance;
        fmx = pot * dx;
        fmy = pot * dy;
    }
    return Vector (fmx, fmy);
}

Vector GetInterParticleForce(const Particle& p1, const Particle& p2, const PhysicalConstants& Params)
{
    double dx = p1.Position.Get(0) - p2.Position.Get(0);
    double dy = p1.Position.Get(1) - p2.Position.Get(1);
    double distance = sqrt (dx * dx + dy * dy);
    double reziR = 1.0 / distance;

    // Gravity
    Vector gravity = GetGravity(p1, p2, reziR, Params.GravitationalConstant);

    // Coulomb and Lorentz
    Vector electromagnetic = GetElectromagneticForce(p1, p2, reziR, Params.ElectrostaticConstant, Params.MagneticPermeability);

    // Lennard-Jones Potential Force
    Vector molecular = GetLennardJonesForce(p1, p2, distance, reziR, Params.AtomicRadius, Params.MolecularBondingEnergy);

    Vector resultingForce = gravity + electromagnetic + molecular;

    return resultingForce;
}

double eKin(const ParticleManager* PManager, const Config & Params) {
    double En = 0.0;
    for (int n1 = 0; n1 < PManager->PCount(); n1++) {
        En += PManager->P(n1).Mass * pow(PManager->P(n1).Velocity.Abs(),2);
    }
    En *= 0.5;
    return(En);
}

double eEl(const ParticleManager* PManager, const Config & Params) {
    double En = 0.0;
    for (int n1 = 0; n1 < PManager->PCount(); n1++) {
        for(int n2=n1+1; n2 < PManager->PCount(); n2++) {
            En+=PManager->P(n1).Charge * PManager->P(n2).Charge / PManager->Distance(n1,n2);
        }
    }
    En *= Params.PhysConstants.ElectrostaticConstant;
    return(En);
}

double eG(const ParticleManager* PManager, const Config & Params) {
    double En = 0.0;
    for (int n1 = 0; n1 < PManager->PCount(); n1++) {
        for(int n2=n1+1; n2 < PManager->PCount(); n2++) {
            En+=PManager->P(n1).Mass * PManager->P(n2).Mass / PManager->Distance(n1,n2);
        }
    }
    En *= - Params.PhysConstants.GravitationalConstant;
    return(En);
}

double eMol(const ParticleManager* PManager, const Config & Params) {
    double En = 0.0;
    for (int n1 = 0; n1 < PManager->PCount(); n1++) {
        for(int n2=n1+1; n2 < PManager->PCount(); n2++) {
            double r = PManager->Distance(n1,n2);
            En+= pow(Params.PhysConstants.AtomicRadius / r, 12) - pow(Params.PhysConstants.AtomicRadius / r, 6) ;
        }
    }
    En *= 4 * Params.PhysConstants.MolecularBondingEnergy;
    return(En);
}
} // namespace Physics
