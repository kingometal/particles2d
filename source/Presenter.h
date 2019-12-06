#ifndef PRESENTER_H
#define PRESENTER_H
#include "interfaces/IPresenter.h"

class Presenter: public IPresenter
{
public:
    Presenter();
    ~Presenter();

    // IPresenter interface
    void StorePoint(int x, int y, const RGBData &data);
    void StoreLine(int x, int y, int dx, int dy, const RGBData &data);
    void Present(int maxFps);
    void Init(int height, int width, bool resizable);
    void ClearWindow(RGBData& color);
    void StoreRectangle(int x, int y, int dx, int dy, const RGBData &data);
private:
    class PresenterImpl;
    PresenterImpl* Pimpl;
};

#endif // DEMO_PRESENTER_H
