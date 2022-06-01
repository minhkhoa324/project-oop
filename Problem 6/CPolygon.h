#pragma once

#include <memory>
#include <Windows.h>
#include <cstdio>
#include "CShape.h"
#include "CPoint.h"
#include "CLine.h"

class CPolygon : 
	public CShape
{
public:
	CPolygon() { side = 0; }
	CPolygon(int n) { side = n; };
	std::string getName() { return "Polygon"; }
	RECT getRect();
	bool inside(CPoint q);
	bool intersect(CLine l);
	void set(int ith, CPoint q);
	void move(double distance_x, double distance_y);
	bool add(CPoint p);
	void draw(HDC& hdc);
	bool isVaild();
};

