#include "CTringale.h"

CTringale::CTringale(CPoint a, CPoint b, CPoint c)
{
	vertex[0] = a;
	vertex[1] = b;
	vertex[2] = c;
}

bool CTringale::add(CPoint p)
{
	if (side < 3)
	{
		if (side > 0)
		{
			if (p.getX() == vertex[side - 1].getX() && p.getY() == vertex[side - 1].getY())
			{
				return false;
			}
		}
		if (side == 2)
		{
			CLine temp(vertex[0], vertex[1]);
			if (temp.onside(p))
			{
				return false;
			}
		}
		vertex[side] = p;
		++side;
		if (side == 3)
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}
