#pragma once
#include "Vector.h"

class Line
{
private:

	Vector _p1, _p2;

public:

	Line() {
		_p1 = Vector::Zero();
		_p2 = Vector::Zero();
	}

	Line(Vector p1, Vector p2) {
		_p1 = p1;
		_p2 = p2;
	}

	Line(float p1x, float p1y, float p2x, float p2y) {
		_p1 = Vector(p1x, p1y);
		_p2 = Vector(p2x, p2y);
	}

	Vector p1() {
		return _p1;
	}

	Vector p2() {
		return _p2;
	}
};

