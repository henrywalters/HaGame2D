#pragma once
#include "Component.h"
#include "Circle.h"
#include "GameObject.h"

class CircleComponent :
	public Component
{
private:
	
	float _radius;

public:
	
	CircleComponent(float radius) {
		_radius = radius;
	}
	
	float getRadius() {
		return _radius;
	}

	Circle getCircle() {
		return Circle{
			transform->position,
			_radius,
		};
	}
};

