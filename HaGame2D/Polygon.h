#pragma once
#include "Vector.h"

template <size_t _size>
struct Polygon {
	size_t size = _size;
	Vector vertices[_size];
};