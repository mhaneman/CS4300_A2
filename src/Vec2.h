#pragma once
#include <math.h> 

class Vec2
{
    public:
        float x, y;

        Vec2();
        Vec2(float in_x, float in_y);

        bool operator == (const Vec2 & rhs) const;
        bool operator != (const Vec2 & rhs) const;

        Vec2 operator + (const Vec2 & rhs) const;
        Vec2 operator - (const Vec2 & rhs) const;
        Vec2 operator * (const float val) const;
        Vec2 operator / (const float val) const;

        void operator += (const Vec2 & rhs);
        void operator -= (const Vec2 & rhs);
        void operator *= (const float val);
        void operator /= (const float val);

        float distance_to(const Vec2 & rhs) const;
        float magnitude();
        Vec2 normalize();
        Vec2 direction_to(const Vec2& rhs) const;

        
};