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
            Presenter.StoreRGBData(x, y, colorData);
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
    for (int x = 0; x < SideX; x++)
    {
        for (int y = 0; y < SideY; y++)
        {
            Presenter.StoreRGBData(x, y, color);
        }
    }
    Presenter.Present();
}

void ParticlesViewSdlAdapter::DrawParticle(RGBData colorData, int x, int y)
{
    Presenter.StoreRGBData(x-2, y-1, colorData);
    Presenter.StoreRGBData(x-2, y, colorData);
    Presenter.StoreRGBData(x-2, y+1, colorData);
    Presenter.StoreRGBData(x-1, y-2, colorData);
    Presenter.StoreRGBData(x-1, y-1, colorData);
    Presenter.StoreRGBData(x-1, y, colorData);
    Presenter.StoreRGBData(x-1, y+1, colorData);
    Presenter.StoreRGBData(x-1, y+2, colorData);
    Presenter.StoreRGBData(x, y-2, colorData);
    Presenter.StoreRGBData(x, y-1, colorData);
    Presenter.StoreRGBData(x, y, colorData);
    Presenter.StoreRGBData(x, y+1, colorData);
    Presenter.StoreRGBData(x, y+2, colorData);
    Presenter.StoreRGBData(x+1, y-2, colorData);
    Presenter.StoreRGBData(x+1, y-1, colorData);
    Presenter.StoreRGBData(x+1, y, colorData);
    Presenter.StoreRGBData(x+1, y+1, colorData);
    Presenter.StoreRGBData(x+1, y+2, colorData);
    Presenter.StoreRGBData(x+2, y-1, colorData);
    Presenter.StoreRGBData(x+2, y, colorData);
    Presenter.StoreRGBData(x+2, y+1, colorData);
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

