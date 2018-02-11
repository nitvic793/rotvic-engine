#include "Vector3.h"



Vector3f::Vector3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

/// <summary>
/// Linearly interpolatesfrom one vector to another. 
/// </summary>
/// <param name="from">Origin Vector</param>
/// <param name="to">Target Vector</param>
/// <param name="t">Step Size</param>
/// <returns>Interpolated vector</returns>
Vector3f Vector3f::Lerp(const Vector3f & from, const Vector3f & to, float t)
{
	auto lerp = XMVectorLerp(XMVectorSet(from.x, from.y, from.z, 0.f), XMVectorSet(to.x, to.y, to.z, 0.f), t);
	XMFLOAT3 r;
	XMStoreFloat3(&r, lerp);
	return Vector3f(r.x, r.y, r.z);
}

/// <summary>
/// Calculates the distance between two vectors. 
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <returns></returns>
float Vector3f::Distance(const Vector3f & from, const Vector3f & to)
{
	float distance;
	XMStoreFloat(&distance, XMVector3Length(XMVectorSet(to.x, to.y, to.z, 0.f) - XMVectorSet(from.x, from.y, from.z, 0.f)));
	return fabsf(distance);
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
