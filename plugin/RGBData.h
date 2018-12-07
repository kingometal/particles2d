#ifndef RGBDATA_H
#define RGBDATA_H

class RGBData
{
public:
    RGBData(char r, char g, char b, char alpha);
//    RGBData(const RGBData& original);
//    RGBData(const RGBData* original);
    RGBData& operator=(const RGBData& rhs);
    RGBData();
    ~RGBData();

    char GetR() const;
    char GetG() const;
    char GetB() const;
    char GetA() const;

private:
    char R;
    char G;
    char B;
    char A;
};
#endif // RGBDATA_H
