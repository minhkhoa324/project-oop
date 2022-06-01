#pragma once

#include "framework.h"

class CPoint {
private:
	double x;
	double y;
public:
	CPoint();
	CPoint(const CPoint& p);
	CPoint(double xx, double yy);
	double getX() { return x; }
	double getY() { return y; }
	double crossProduct(CPoint p, CPoint r);
	void set(CPoint p);
	void move(double distance_x, double distance_y);
	friend void swap(CPoint& a, CPoint& b);
	friend bool operator < (CPoint& a, CPoint& b);
};

const CPoint origin = CPoint(0, 101);

