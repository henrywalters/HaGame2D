#pragma once
#include "CollisionComponent.h"
#include "GameObject.h"
#include "Circle.h"
#include "CircleComponent.h"

class CircleCollider : public CollisionComponent
{

	double _radius;
	bool _positionCenter;

public:

	CircleCollider(double radius, bool positionCenter = true) {
		_radius = radius;
		_positionCenter = positionCenter;
	}

	Circle getCircle() {
		Vector position = transform->position;

		if (!_positionCenter) position += Vector(_radius, _radius);

		return Circle{
			position,
			_radius,
		};
	}
};

