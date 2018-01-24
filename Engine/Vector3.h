#pragma once


class Vector3f
{	
public:
	float x;
	float y;
	float z;
	Vector3f();
	Vector3f operator+(const Vector3f& in);
	Vector3f(float x, float y, float z);
	~Vector3f();
};



