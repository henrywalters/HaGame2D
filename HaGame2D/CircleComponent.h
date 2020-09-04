#pragma once
#include "Component.h"
#include "Circle.h"
#include "GameObject.h"

class CircleComponent :
	public Component
{
private:
	
	bool _positionCenter;
	float _radius;

public:
	
	CircleComponent(float radius, bool positionCenter) {
		_radius = radius;
		_positionCenter = positionCenter;
	}

	void setPositionCenter(bool positionCenter) {
		_positionCenter = positionCenter;
	}

	void setRadius(float radius) {
		_radius = radius;
	}

	bool getPositionCenter() {
		return _positionCenter;
	}
	
	float getRadius() {
		return _radius;
	}

	Circle getCircle() {

		Vector position = transform->relativePosition;

		if (!getPositionCenter()) position += Vector(getRadius(), getRadius());

		return Circle{
			position,
			_radius,
		};
	}
};

