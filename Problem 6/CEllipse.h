#pragma once

#include "framework.h"
#include "CPoint.h"
#include "CShape.h"

class CEllipse :
	public CShape
{
public:
	CEllipse(CPoint top_left = origin, double length_a = 0,  double length_b = 0);
	std::string getName() { return "Ellipse"; }
	RECT getRect();
	bool inside(CPoint q);
	bool intersect(CLine l);
	void set(int ith, CPoint q);
	void move(double distance_x, double distance_y);
	bool add(CPoint p);
	void draw(HDC& hdc);
	bool isVaild();
};


