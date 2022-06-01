#pragma once
#include "CEllipse.h"

class CCircle :
    public CEllipse
{
private:
    double radius;
public:
    CCircle(CPoint p = origin, double radius = 0) :
        radius{ radius }, 
        CEllipse(p, radius, radius) {}
    std::string getName() { return "Circle"; }
    void draw(HDC& hdc);
};

class SemiCircle :
    public CEllipse
{
public:
    SemiCircle(CPoint p = origin,  double radius = 0) :
        CEllipse(p, radius, radius) {}
    std::string getName() { return "SemiCircle"; }
    bool inside(CPoint q);
    void draw(HDC& hdc);
};

