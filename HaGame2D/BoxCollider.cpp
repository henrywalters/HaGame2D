#include "stdafx.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider(double _width, double _height)
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
