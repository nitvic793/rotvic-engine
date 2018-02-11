#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// Vector to represent a point in 3D space.
/// </summary>
class Vector3f
{	
public:
	float x;
	float y;
	float z;
	static Vector3f Lerp(const Vector3f& from, const Vector3f& to, float t);
	static float Distance(const Vector3f& from, const Vector3f& to);
	Vector3f();
	Vector3f operator+(const Vector3f& in);
	Vector3f(float x, float y, float z);
	~Vector3f();

};



