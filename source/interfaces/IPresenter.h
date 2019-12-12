#ifndef IPRESENTER_H
#define IPRESENTER_H

class RGBData;

class IPresenter
{
public:
    IPresenter() = default;
    ~IPresenter() = default;
    virtual bool Init(int height, int width) = 0;
    virtual void StorePoint(int x, int y, const RGBData& data) = 0;
    virtual void StoreLine(int x, int y, int dx, int dy, const RGBData &data) = 0;
    virtual void StoreRectangle(int x, int y, int dx, int dy, const RGBData &data) = 0;
    virtual void StoreText(const char* text, const RGBData &color, int x, int y) = 0;
    virtual void Present() = 0;
    virtual void ClearWindow(RGBData& color) = 0;
    virtual void GetWindowSize(int &x, int &y) const = 0;
};

#endif // IPRESENTER_H
