#include "CRectangle.h"


CRectangle::CRectangle(CPoint top_left, double n_width, double n_height)
{
	side = 2;
	vertex[0] = top_left;
	vertex[1] = CPoint(top_left.getX() + n_width, top_left.getY() + n_height);
}

bool CRectangle::intersect(CLine l)
{
	CPolygon temp;
	double width = vertex[1].getX() - vertex[0].getX();
	double height = vertex[1].getY() - vertex[0].getY();
	double top = vertex[0].getX();
	double left = vertex[0].getY();
	temp.add(CPoint(top, left));
	temp.add(CPoint(top + width, left));
	temp.add(CPoint(top + width, left + height));
	temp.add(CPoint(top, left + height));
	return temp.intersect(l);
}

bool CRectangle::isVaild()
{
	if (vertex[0].getX() == vertex[1].getX() &&
		vertex[0].getY() == vertex[1].getY())
	{
		return false;
	}
	return true;
}

void CRectangle::draw(HDC& hdc)
{
	int x1 = vertex[0].getX();
	int y1 = vertex[0].getY();
	int x2 = vertex[1].getX();
	int y2 = vertex[1].getY();
	Rectangle(hdc, x1, y1, x2, y2);
}
