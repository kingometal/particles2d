#include "ParticlesViewSdlAdapter.h"
#include "interfaces/IPresenter.h"
#include "Constants.h"
#include "RGBData.h"
#include <chrono>

ParticlesViewSdlAdapter::ParticlesViewSdlAdapter(IPresenter& presenter, int sideX, int sideY)
    : Presenter(presenter)
    , SideX(sideX)
    , SideY(sideY)
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

void ParticlesViewSdlAdapter::DrawParticle(int /*index*/, EColor color, int x, int y)
{
    DrawParticle(color, x, y);
}

void ParticlesViewSdlAdapter::ClearWindow()
{
    RGBData white (255,255,255,0);
    for (int x = 0; x < SideX; x++)
    {
        for (int y = 0; y < SideY; y++)
        {
            Presenter.StoreRGBData(x, y, white);
        }
    }
    Presenter.Present();
}

void ParticlesViewSdlAdapter::DrawParticle(EColor color, int x, int y)
{
    RGBData colorData;
    switch (color)
    {
    case EColor::Black:
        colorData = RGBData(0,0,0,0);
        break;
    case EColor::White:
        colorData = RGBData(255,255,255,255);
        break;
    case EColor::Red:
        colorData = RGBData(255,0,0,255);
        break;
    case EColor::Blue:
        colorData = RGBData(0,0,255,255);
        break;

    }
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
//    long long currentTime =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//    if (currentTime - LastUpdate > 20)
//    {
//        LastUpdate=currentTime;
        Presenter.Present(MAX_FPS);
//    }
}

int ParticlesViewSdlAdapter::GetSideX()
{
    return SideX;
}

int ParticlesViewSdlAdapter::GetSideY()
{
    return SideY;
}

