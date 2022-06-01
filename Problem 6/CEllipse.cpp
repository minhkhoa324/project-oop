#include "CEllipse.h"

#define sqr(a) (a) * (a)

CEllipse::CEllipse(CPoint top_left, double length_a, double length_b)
{
	side = 2;
	vertex[0] = top_left;
	vertex[1] = CPoint((int)top_left.getX() + length_a * 2, (int)top_left.getY() + length_b * 2);
	if (vertex[1] < vertex[0])
	{
		swap(vertex[0], vertex[1]);
	}
}

RECT CEllipse::getRect()
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

void CEllipse::move(double distance_x, double distance_y) 
{
	for (int i = 0; i < side; ++i)
	{
		if (vertex[i].getY() + distance_y <= MIN_Y || vertex[i].getX() + distance_x < 0)
		{
			return;
		}
	}
	vertex[0].move(distance_x, distance_y);
	vertex[1].move(distance_x, distance_y);
}

void CEllipse::set(int ith, CPoint q) 
{
	vertex[ith] = q;
}

bool CEllipse::add(CPoint p)
{
	// do something
	return true;
}

bool CEllipse::inside(CPoint p) 
{
	double x_center = fabs(vertex[0].getX() + vertex[1].getX()) / 2;
	double y_center = fabs(vertex[0].getY() + vertex[1].getY()) / 2;
	double len_a = fabs(vertex[0].getX() - vertex[1].getX()) / 2;
	double len_b = fabs(vertex[0].getY() - vertex[1].getY()) / 2;
	double first_ar = (sqr(p.getX() - x_center)) / (sqr(len_a));
	double second_ar = (sqr(p.getY() - y_center)) / (sqr(len_b));
	return (first_ar + second_ar <= 1);
}

bool CEllipse::intersect(CLine l)
{
	double max_x = 3000;
	for (int i = 0; i <= max_x; ++i)
	{
		double j = (-l.getA() * i + l.getC()) / l.getB();
		if (this->inside(CPoint(i, j)))
		{
			return true;
		}
	}
	return false;
}

bool CEllipse::isVaild()
{
	if (vertex[0].getX() == vertex[1].getX() &&
		vertex[0].getY() == vertex[1].getY())
	{
		return false;
	}
	return true;
}

void CEllipse::draw(HDC& hdc) 
{
	Ellipse(hdc, (int) vertex[0].getX(), (int) vertex[0].getY(), 
		(int) vertex[1].getX(), (int) vertex[1].getY());
}
