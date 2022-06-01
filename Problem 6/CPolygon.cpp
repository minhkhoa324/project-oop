#include "CPolygon.h"

void CPolygon::set(int ith, CPoint p) 
{
	vertex[ith] = p;
}

RECT CPolygon::getRect()
{
	int min_x = 2000, min_y = 2000;
	int max_x = 0, max_y = 0;
	for (int i = 0; i < side; ++i)
	{
		min_x = min(min_x, (int)vertex[i].getX());
		min_y = min(min_y, (int)vertex[i].getY());
		max_x = max(max_x, (int)vertex[i].getX());
		max_y = max(max_y, (int)vertex[i].getY());
	}
	return { min_x - 4, min_y - 4, max_x + 4, max_y + 4 };
}

bool CPolygon::inside(CPoint q)
{
	if (side < 3)
	{
		return false;
	}

	int pos = 0; 
	int neg = 0;

	for (int i = 0; i < side; ++i)
	{
		// If q is polygon's vertex, return true
		if (vertex[i].getX() == q.getX() && vertex[i].getY() == q.getY())
		{
			return true;
		}

		// And the i + 1'th, or if i is the last, with the first point
		int i2 = (i + 1) % side;

		// Compute the cross product
		double d = q.crossProduct(vertex[i], vertex[i2]);
		if (d > 0)
		{
			pos++;
		}
		if (d < 0) {
			neg++;
		}

		//If the sign changes, then point is outside
		if (pos > 0 && neg > 0) {
			return false;
		}
	}

	// If no change in direction, then on same side of all segments, and thus inside
	return true;
}

bool CPolygon::intersect(CLine l)
{
	double max_x = l.getX(1);
	double max_y = l.getY(1);
	for (int i = 0; i <= 3000; ++i)
	{
		double j = (-l.getA() * i + l.getC()) / l.getB();
		if (this->inside(CPoint(i, j)))
		{
			return true;
		}
	}
	return false;
}

void CPolygon::move(double distance_x, double distance_y) 
{
	for (int i = 0; i < side; ++i)
	{
		if (vertex[i].getY() + distance_y <= MIN_Y ||
			vertex[i].getX() + distance_x < 0)
		{
			return;
		}
	}
	for (int i = 0; i < side; ++i) {
		vertex[i].move(distance_x, distance_y);
	}
}

bool CPolygon::add(CPoint p) 
{
	vertex[side] = p;
	side++;
	if (side > 3) {

		// You should check if the polygon is convex
		bool pos = false;
		bool neg = false;
		for (int i = 0; i < side; ++i)
		{
			int i2 = (i + 1) % side;
			int i3 = (i + 2) % side;
			double crossProductValue = vertex[i].crossProduct(vertex[i2], vertex[i3]);
			if (crossProductValue < 0)
			{
				neg = true;
			}
			if (crossProductValue > 0)
			{
				pos = true;
			}
		}
		if (pos & neg)
		{
			--side;
			return false;
		}
	}
	return true;
}

bool CPolygon::isVaild()
{
	if (side < 3)
	{
		return false;
	}
	bool flags = false;
	for (int i = 0; i < side - 2; ++i)
	{
		CLine temp(vertex[i + 1], vertex[i + 2]);
		if (!temp.onside(vertex[i]))
		{
			flags = true;
		}
	}
	return flags;
}

void CPolygon::draw(HDC& hdc) 
{
	POINT temp[MAX_SIDE];
	for (int i = 0; i < side; ++i) 
	{
		temp[i].x = (int)vertex[i].getX();
		temp[i].y = (int)vertex[i].getY();
	}
	Polygon(hdc, temp, side);
}
