#include "ParticlesViewSdlAdapter.h"
#include "interfaces/IPresenter.h"
#include "RGBData.h"
#include <chrono>

ParticlesViewSdlAdapter::ParticlesViewSdlAdapter(IPresenter& presenter, int sideX, int sideY, int maxFPS)
    : Presenter(presenter)
    , SideX(sideX)
    , SideY(sideY)
    , MaxFPS(maxFPS)
{
    Presenter.Init(sideY, sideX, false);
    RGBData colorData(255,255,255,0);
    for (int x = 0; x < sideX; x++)
    {
        for (int y = 0; y < sideY; y++)
        {
            Presenter.StorePoint(x, y, colorData);
        }
    }
    Presenter.Present();
}

void ParticlesViewSdlAdapter::DrawParticle(int /*index*/, RGBData color, int x, int y)
{
    DrawParticle(color, x, y);
}

void ParticlesViewSdlAdapter::ClearWindow(RGBData color)
{
    Presenter.StoreRectangle(0, 0, SideX, SideY, color);
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

int count = 0;
long long LastUpdate = 0;

void ParticlesViewSdlAdapter::DrawScreen()
{
    Presenter.Present(MaxFPS);
}

int ParticlesViewSdlAdapter::GetSideX()
{
    return SideX;
}

int ParticlesViewSdlAdapter::GetSideY()
{
    return SideY;
}

