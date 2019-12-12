#ifndef RGBDATA_H
#define RGBDATA_H

class RGBData
{
public:
    RGBData(char r, char g, char b, char alpha);
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
