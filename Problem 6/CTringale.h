#pragma once
#include "CPolygon.h"
class CTringale :
    public CPolygon
{
public:
    CTringale(CPoint a = origin, CPoint b = origin, CPoint c = origin);
    std::string getName() { return "Triangle"; }
    bool add(CPoint p);
};

