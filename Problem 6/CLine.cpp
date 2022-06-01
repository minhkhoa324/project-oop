#include "CLine.h"

CLine::CLine() 
{
	p[0] = CPoint(0, 0);
	p[1] = CPoint(0, 0);
}

CLine::CLine(const CLine& g)
{
	p[0] = g.p[0];
	p[1] = g.p[1];
}

CLine::CLine(CPoint a, CPoint b) 
{
	p[0] = a;
	p[1] = b;
}

bool CLine::isVaild()
{
	if (p[0].getX() == 0 && p[0].getY() == 0 && p[1].getX() == 0 && p[1].getY() == 0)
	{
		return false;
	} 
	else
	{
		return true;
	}
}

double CLine::getA() 
{
	return p[1].getY() - p[0].getY();
}

double CLine::getB() 
{
	return p[0].getX() - p[1].getX();
}

double CLine::getC() 
{
	return this->getA() * p[0].getX() + this->getB() * p[0].getY();
}

void CLine::set(int ith, CPoint q) 
{
	p[ith] = q;
}

double CLine::getX(int ith) 
{
	return p[ith].getX();
}

double CLine::getY(int ith) 
{
	return p[ith].getY();
}

bool CLine::onside(CPoint p) 
{
	double xx = p.getX();
	double yy = p.getY();
	return (this->getA() * xx + this->getB() * yy - this->getC() == 0);
}


void CLine::draw(HDC& hdc) 
{
	POINT temp[2];
	for (int i = 0; i < side; ++i) {
		temp[i].x = p[i].getX();
		temp[i].y = p[i].getY();
	}
	Polyline(hdc, temp, side);
}

