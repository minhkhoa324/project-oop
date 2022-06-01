#pragma once
#include "CPolygon.h"
class CRectangle :
    public CPolygon
{
public:
    CRectangle(CPoint top_left = origin, double n_width = 0, double n_height = 0);
    std::string getName() { return "Rectangle"; }
    bool intersect(CLine l);
    bool isVaild();
	void draw(HDC& hdc);
};


