#include "Vec2.h"

Vec2::Vec2() {}

Vec2::Vec2(float _x, float _y)
    : x(_x), y(_y) {}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return (x != rhs.x && y != rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x*val, y*val);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x/val, y/val);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
    x *= val;
    y *= val;
}

void Vec2::operator /= (const float val)
{
    x /= val;
    y /= val;
}

float Vec2::distance_to(const Vec2& rhs) const
{
    return sqrt(powf(x - rhs.x, 2) + powf(y - rhs.y, 2));
}

float Vec2::magnitude()
{
    return sqrt(powf(x, 2) + powf(y, 2));
}

Vec2 Vec2::normalize()
{
    auto mag = magnitude();
    return Vec2(x / mag, y / mag);
}

Vec2 Vec2::direction_to(const Vec2& rhs) const
{
    return Vec2(rhs.x - x, rhs.y - y).normalize();
}