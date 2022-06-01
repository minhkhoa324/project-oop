#pragma once

#include "framework.h"
#include "CPoint.h"
#include "CLine.h"

const int MAX_SIDE = 500;
const int MIN_Y = 100;

class CShape {
protected:
	int side;
	CPoint vertex[MAX_SIDE];
public:
	CShape() { side = 0; }
	int getSide() { return side; }
	// Return name of the shape
	virtual std::string getName() = NULL;
	// Return a rectangle cover this shape
	virtual RECT getRect() = NULL; 
	// Return true if the point is inside this shape, else return false
	virtual bool inside(CPoint q) = NULL;
	// Change vertex(x, y) to q(x2, y2)
	virtual void set(int ith, CPoint q) = NULL;
	// Move every vertex(xi, yi) to vertex(xi + distance_x, yi + distance_y)
	virtual void move(double distance_x, double distance_y) = NULL;
	// add a new point to this shape
	virtual bool add(CPoint p) = NULL;
	// draw the shape
	virtual void draw(HDC& hdc) = NULL; 
	// return true if the shape intersect with the line l
	virtual bool intersect(CLine l) = NULL;
	// return true if the shape is vail, else return false
	virtual bool isVaild() = NULL;
};
