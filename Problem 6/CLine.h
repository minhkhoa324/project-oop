#pragma once

#include "framework.h"
#include "CPoint.h"

class CLine {
private:
	const int side = 2;
	CPoint p[2];
public:
	CLine();
	CLine(const CLine& g);
	CLine(CPoint a, CPoint b);
	void set(int ith, CPoint q);
	CPoint get(int ith) { return p[ith]; }
	bool isVaild();
	double getX(int ith);
	double getY(int ith);
	double getA();
	double getB();
	double getC();
	bool onside(CPoint p);
	void draw(HDC& hdc);
};

