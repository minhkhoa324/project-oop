#pragma once
#include "CRectangle.h"

class CSquare :
    public CRectangle
{
public:
    CSquare(CPoint top_left = origin, double len = 0) :
        CRectangle(top_left, len, len) {}
    std::string getName() { return "Square"; }
};

