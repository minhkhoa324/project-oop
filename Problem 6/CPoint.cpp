#include "CPoint.h"

CPoint::CPoint() 
{
	x = 0;
	y = 0;
}

CPoint::CPoint(double xx, double yy)
{
	x = xx;
	y = yy;
}

CPoint::CPoint(const CPoint& q)
{
	x = q.x;
	y = q.y;
}

void CPoint::set(CPoint p)
{
	x = p.x;
	y = p.y;
}

double CPoint::crossProduct(CPoint p, CPoint r) 
{
	double BAx = x - p.getX();
	double BAy = y - p.getY();
	double BCx = r.getX() - p.getX();
	double BCy = r.getY() - p.getY();
	return (BAx * BCy - BAy * BCx);
}

void CPoint::move(double distance_x, double distance_y) 
{
	x += distance_x;
	y += distance_y;
}

void swap(CPoint& a, CPoint& b)
{
	CPoint temp = a;
	a = b;
	b = temp;
}

bool operator < (CPoint& a, CPoint& b)
{
	return (a.x < b.x && a.y < b.y);
}
