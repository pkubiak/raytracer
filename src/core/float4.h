#pragma once

#include <core/macros.h>

namespace rt {

class Point;
class Vector;

class ALIGN(16) Float4 {
public:

    Float4() {}
    Float4(float x, float y, float z, float w);
    explicit Float4(const Point& p);
    explicit Float4(const Vector& v);

    static Float4 rep(float v) { return Float4(v,v,v,v); }

    float& operator[](int idx);
    float operator[](int idx) const;

    Float4 operator+(const Float4& b) const;
    Float4 operator-(const Float4& b) const;
    Float4 operator*(const Float4& b) const;
    Float4 operator/(const Float4& b) const;


    Float4 operator-() const;

    bool operator==(const Float4& b) const;
    bool operator!=(const Float4& b) const;

private:
    float v[4];

};

Float4 operator*(float scalar, const Float4& b);
Float4 operator*(const Float4& a, float scalar);
Float4 operator/(const Float4& a, float scalar);
float dot(const Float4& a, const Float4& b);

Float4 min(const Float4& a, const Float4& b);
Float4 max(const Float4& a, const Float4& b);

}
