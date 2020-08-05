#ifndef RGBDATA_H
#define RGBDATA_H

class RGBData
{
public:
    RGBData(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);
    RGBData& operator=(const RGBData& rhs);
    RGBData();
    ~RGBData();

    unsigned char GetR() const;
    unsigned char GetG() const;
    unsigned char GetB() const;
    unsigned char GetA() const;

private:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};
#endif // RGBDATA_H
