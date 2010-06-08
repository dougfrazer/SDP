#ifndef _VECMATH_H_
#define _VECMATH_H_

#include <math.h>
#include <stdio.h>
#include "poolfiz.h"

class vec2
{
	public:
	vec2(double x, double y) { this->x = x; this->y = y; }
	vec2() { x = 0; y = 0; }
	vec2(fizPoint p) { x = p.x; y = p.y; }
	vec2(const vec2& rhs) { x = rhs.x; y = rhs.y; }
	double x;
	double y;
	
	void print()
	{
		printf("x: %f\ty:%f\n",x,y);
	}
	
	double length()
	{
		return sqrt(x * x + y * y);
	}
	
	void normalize(double l = 1.0f)
	{
		double factor = l / length();
		x *= factor;
		y *= factor;
	}
	
	double dot(vec2& rhs)
	{
		return x * rhs.x + y * rhs.y;
	}
	
	double angleDegrees(vec2& rhs)
	{
		vec2 thisn = *this;
		vec2 rhsn = rhs;
		thisn.normalize(1);
		rhsn.normalize(1);
		double dotprod = thisn.dot(rhsn);
		//printf("dotprod %f\n",dotprod);
		double angle = acos(dotprod);
		//printf("angle radians: %f\n", angle);
		angle *= 180 / 3.14159;
		//printf("angle degrees: %f\n",angle);
		return angle;
	}
	
	double angleRad(vec2& rhs)
	{
		vec2 thisn = *this;
		vec2 rhsn = rhs;
		thisn.normalize(1);
		rhsn.normalize(1);
		double dotprod = thisn.dot(rhsn);
		//printf("dotprod %f\n",dotprod);
		double angle = acos(dotprod);
		//printf("angle radians: %f\n", angle);
		//printf("angle degrees: %f\n",angle);
		return angle;
	}
	
	vec2 rotateDegrees(double degrees)
	{
		double radians = degrees * 3.14159 / 180;
		return rotate(radians);
	}
		
	vec2 rotate(double theta)
	{
		double x1 = cos(theta) * x - sin(theta) * y;
		double y1 = sin(theta) * x + cos(theta) * y;
		
		return vec2(x1,y1);
	}
	
	vec2 operator+(vec2& b)
	{
		return vec2(x + b.x, y + b.y);
	}
	
	vec2 operator-(vec2& b)
	{
		return vec2(x - b.x, y - b.y);
	}
	
	vec2 operator-()
	{
		return vec2(-x, -y);
	}
	
	bool operator==(const vec2& rhs)
	{
		return ((rhs.x == x) && (rhs.y == y));
	}
	
	vec2& operator=(const vec2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	
	fizPoint getFizPoint()
	{
		return fizPoint(x,y);
	}
};

#endif

