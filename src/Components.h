#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
    Vec2    position    = {0.0f, 0.0f};
    Vec2    velocity    = {0.0f, 0.0f};
    float   angle       = 0;

    CTransform(const Vec2 & _position, const Vec2& _velocity, float _angle)
        : position(_position), velocity(_velocity), angle(_angle) {}
};

class CShape
{
public:
    sf::CircleShape circle;

    CShape(float _radius, int _points, const sf::Color& _fill, const sf::Color& _outline, float _thickness)
        : circle(_radius, _points)
    {
        circle.setFillColor(_fill);
        circle.setOutlineColor(_outline);
        circle.setOutlineThickness(_thickness);
        circle.setOrigin(_radius, _radius);
    }
};

class CCollision
{
public:
    float radius = 0;
    CCollision(float _radius) : radius(_radius){}
};

class CInput
{
public:
    bool up     = false;
    bool down   = false; 
    bool left   = false; 
    bool right  = false; 
    bool shoot  = false;

    CInput() {}     
};

class CScore
{
    int score = 0;
    CScore(int _score) : score(_score){}
};

class CLifespan
{
public:
    int remaining   = 0; // amount of lifespan remaining on the entity
    int total       = 0; // the total initial amout of lifespan

    CLifespan(int _total) : total(_total) {} 
};