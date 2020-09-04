#include "stdafx.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider(float _width, float _height)
{
	width = _width;
	height = _height;
}

BoxCollider::BoxCollider(Vector size)
{
	width = size.x;
	height = size.y;
}


BoxCollider::~BoxCollider()
{
}
