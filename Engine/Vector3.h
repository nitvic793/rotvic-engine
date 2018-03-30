#pragma once
#define NOMINMAX
#undef min
#undef max
#include <DirectXMath.h>
#include "reactphysics3d.h"

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
	Vector3f Vector3f::operator=(rp3d::Vector3 in);
	rp3d::Vector3 rp3dVec3();
	Vector3f(float x, float y, float z);
	Vector3f(rp3d::Vector3 vec);
	~Vector3f();

};



