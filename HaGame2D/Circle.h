#pragma once

class Circle {
private:
	float _radius;

public:

	Circle() {
		_radius = 0;
	}

	Circle(float radius) {
		_radius = radius;
	}

	void setRadius(float radius) {
		_radius = radius;
	}

	float getRadius() {
		return _radius;
	}

};