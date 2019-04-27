#include "RGBData.h"
#include "iostream"

RGBData::RGBData(char r, char g, char b, char alpha)
    : R(r)
    , G(g)
    , B(b)
    , A(alpha)
{
}

RGBData &RGBData::operator=(const RGBData& rhs)
{
    this->R = rhs.R;
    this->G = rhs.G;
    this->B = rhs.B;
    this->A = rhs.A;
    return *this;
}

RGBData::RGBData()
    : R(0)
    , G(0)
    , B(0)
    , A(0)
{
//    std::cout << " c " << std::endl;
}

RGBData::~RGBData()
{
}

char RGBData::GetR() const
{
    return R;
}

char RGBData::GetG() const
{
    return G;
}

char RGBData::GetB() const
{
    return B;
}

char RGBData::GetA() const
{
    return A;
}
