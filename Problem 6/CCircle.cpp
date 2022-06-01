#include "CCircle.h"

void CCircle::draw(HDC& hdc)
{
	Ellipse(hdc, vertex[0].getX(), vertex[0].getY(), vertex[1].getX(), vertex[1].getY());
}

bool SemiCircle::inside(CPoint q)
{
	// Specially, the direction of our semi-circle is down

	if (this->CEllipse::inside(q))
	{
		//Compute the cross product

		double d = q.crossProduct(vertex[0], vertex[1]);
		if (d < 0)
		{
			return true;
		}
	}
	return false;
}

void SemiCircle::draw(HDC& hdc)
{
	int x1 = vertex[0].getX();
	int y1 = vertex[0].getY();
	int x2 = vertex[1].getX();
	int y2 = vertex[1].getY();
	Chord(hdc, x1, y1, x2, y2, x1, y1, x2, y2);
}
