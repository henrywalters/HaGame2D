#pragma once
#include "Component.h"
#include "Circle.h"

class CircleComponent :
	public Component
{
private:

	Circle _circle;

public:
	
	CircleComponent(float radius) {
		_circle = Circle(radius);
	}

	Circle getCircle() {
		return _circle;
	}

	void setCircle(Circle circle) {
		_circle = circle;
	}
};

