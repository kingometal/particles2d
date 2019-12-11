#include "ParticlesViewSdlAdapter.h"
#include "interfaces/IPresenter.h"
#include "RGBData.h"
#include <chrono>

ParticlesViewSdlAdapter::ParticlesViewSdlAdapter(IPresenter& presenter, int sideX, int sideY, int maxFPS)
    : Presenter(presenter)
    , MaxFPS(maxFPS)
{
    Presenter.Init(sideX, sideY, true);
    RGBData colorData(255,255,255,0);
    ClearWindow(colorData);
    Presenter.Present();
}

void ParticlesViewSdlAdapter::DrawParticle(int /*index*/, RGBData color, int x, int y)
{
    DrawParticle(color, x, y);
}

void ParticlesViewSdlAdapter::ClearWindow(RGBData color)
{
    Presenter.ClearWindow(color);
}

void ParticlesViewSdlAdapter::DrawParticle(RGBData colorData, int x, int y)
{
    Presenter.StorePoint(x-2, y-1, colorData);
    Presenter.StorePoint(x-2, y, colorData);
    Presenter.StorePoint(x-2, y+1, colorData);
    Presenter.StorePoint(x-1, y-2, colorData);
    Presenter.StorePoint(x-1, y-1, colorData);
    Presenter.StorePoint(x-1, y, colorData);
    Presenter.StorePoint(x-1, y+1, colorData);
    Presenter.StorePoint(x-1, y+2, colorData);
    Presenter.StorePoint(x, y-2, colorData);
    Presenter.StorePoint(x, y-1, colorData);
    Presenter.StorePoint(x, y, colorData);
    Presenter.StorePoint(x, y+1, colorData);
    Presenter.StorePoint(x, y+2, colorData);
    Presenter.StorePoint(x+1, y-2, colorData);
    Presenter.StorePoint(x+1, y-1, colorData);
    Presenter.StorePoint(x+1, y, colorData);
    Presenter.StorePoint(x+1, y+1, colorData);
    Presenter.StorePoint(x+1, y+2, colorData);
    Presenter.StorePoint(x+2, y-1, colorData);
    Presenter.StorePoint(x+2, y, colorData);
    Presenter.StorePoint(x+2, y+1, colorData);
}

void ParticlesViewSdlAdapter::DrawLine(RGBData color, int x, int y, int dx, int dy)
{
    Presenter.StoreLine(x, y, dx, dy, color);
}

void ParticlesViewSdlAdapter::DrawScreen()
{
    Presenter.Present(MaxFPS);
}

int ParticlesViewSdlAdapter::GetSideX()
{
    int x, y;
    Presenter.GetWindowSize(x, y);
    return x;
}

int ParticlesViewSdlAdapter::GetSideY()
{
    int x, y;
    Presenter.GetWindowSize(x, y);
    return y;
}

