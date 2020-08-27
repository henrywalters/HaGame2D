#pragma once
#include "CollisionComponent.h"
#include "GameObject.h"
#include "Circle.h"

class CircleCollider :
	public CollisionComponent
{
private:
	float _radius;

public:

	CircleCollider(float radius) {
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

