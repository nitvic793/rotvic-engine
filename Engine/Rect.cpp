#include "Rect.h"



Rect::Rect()
{
	x = y = height = width = 0.0f;
}

Rect::Rect(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

Rect::~Rect()
{
}
