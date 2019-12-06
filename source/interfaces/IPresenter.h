#ifndef IPRESENTER_H
#define IPRESENTER_H

class RGBData;

class IPresenter
{
public:
    IPresenter() = default;
    ~IPresenter() = default;
    virtual void Init(int height, int width, bool resizable) = 0;
    virtual void StorePoint(int x, int y, const RGBData& data) = 0;
    virtual void StoreLine(int x, int y, int dx, int dy, const RGBData &data) = 0;
    virtual void StoreRectangle(int x, int y, int dx, int dy, const RGBData &data) = 0;
    virtual void Present(int maxFps = 0) = 0;
    virtual void ClearWindow(RGBData& color) = 0;
};

#endif // IPRESENTER_H
