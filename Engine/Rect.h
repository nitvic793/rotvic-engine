#pragma once
#define NOMINMAX
class Rect
{
public:
	float x;
	float y;
	float width;
	float height;
	Rect();
	Rect(float x, float y, float width, float height);
	~Rect();
};

