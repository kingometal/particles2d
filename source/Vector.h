#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector
{

public:
    double v[2];

    Vector()
        :v{0.0, 0.0}
    {
    }

    Vector(const Vector& v2)
        :v{v2.v[0], v2.v[1]}
    {
    }

    Vector(double x, double y)
        :v{x, y}
    {
    }

    inline double Get(int dim) const
    {
        return v[dim];
    }

    inline double X() const
    {
        return v[0];
    }

    inline double Y() const
    {
        return v[1];
    }

    inline void Set(const double x, const double y)
    {
        v[0] = x;
        v[1] = y;
   }

    inline Vector operator+(const Vector& v2) const
    {
        Vector temp (*this);
        temp.v[0] += v2.v[0];
        temp.v[1] += v2.v[1];
        return temp;
    }

    inline Vector& operator+=(const Vector& v2)
    {
        v[0] += v2.v[0];
        v[1] += v2.v[1];
        return *this;
    }

    inline Vector operator-(const Vector& v2) const
    {
        Vector temp (*this);
        temp.v[0] -= v2.v[0];
        temp.v[1] -= v2.v[1];
        return temp;
    }

    inline Vector operator*(const double f) const
    {
        Vector temp (*this);
        temp.v[0] *= f;
        temp.v[1] *= f;
        return temp;
    }

    inline void Reset()
    {
        Set(0.0, 0.0);
    }

    inline Vector& operator=(const Vector& v2)
    {
        v[0] = v2.v[0];
        v[1] = v2.v[1];
    }

    inline double Abs() const
    {
        return sqrt(v[0] * v[0] + v[1] * v[1]);
    }
};

#endif // VECTOR_H
