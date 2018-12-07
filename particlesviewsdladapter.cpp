#include "particlesviewsdladapter.h"
#include "plugin/IPresenter.h"
#include "plugin/RGBData.h"

particlesViewSdlAdapter::particlesViewSdlAdapter(IPresenter& presenter, int side)
    : Presenter(presenter)
    , Side(side)
{
    Presenter.Init(side, side, false);
    RGBData colorData(255,255,255,0);
    for (int x = 0; x < side; x++)
    {
        for (int y = 0; y < side; y++)
        {
            Presenter.StoreRGBData(x, y, colorData);
        }
    }
    Presenter.Present();
}

void particlesViewSdlAdapter::DrawParticle(int /*index*/, EColor color, int x, int y)
{
    DrawParticle(color, x, y);
}

void particlesViewSdlAdapter::DrawParticle(EColor color, int x, int y)
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

void particlesViewSdlAdapter::DrawScreen()
{
    Presenter.Present();
}

char particlesViewSdlAdapter::CheckKeyPress()
{
    return 0;
}

int particlesViewSdlAdapter::GetSide()
{
    return Side;
}

