#include "Vector3.h"



Vector3f::Vector3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3f::Vector3f()
{
	x = y = z = 0.f;
}

Vector3f::~Vector3f()
{
}

Vector3f Vector3f::operator+(const Vector3f& in)
{
	x = x + in.x;
	y = y + in.y;
	z = z + in.z;
	return *this;
}
