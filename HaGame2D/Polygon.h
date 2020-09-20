#pragma once
#include "Vector.h"
#include "Box.h"

template <size_t _size>
struct Polygon {
	size_t size = _size;
	Vector vertices[_size];

	Box getBoundingBox() {
		Box box = Box{ 0, 0, 0, 0 };
		if (size == 0) return box;
		box = Box{ vertices[0].x, vertices[0].y, 1, 1 };

		for (int i = 1; i < size; i++) {
			if (vertices[i].x < box.x) box.x = vertices[i].x;
			if (vertices[i].y < box.y) box.y = vertices[i].y;
			if (vertices[i].x > box.x + box.width) box.width = vertices[i].x - box.x;
			if (vertices[i].y > box.y + box.height) box.height = vertices[i].y - box.y;
		}

		return box;
	}
};