#pragma once
#include "Box.h"
#include "Vector.h"

struct Box;

struct Circle {
	Vector center;
	float radius;

	bool collidingWithBox(Box box) {
		Vector closestPoint = Vector(
			Math::clamp(center.x, box.x, box.x + box.width),
			Math::clamp(center.y, box.y, box.y + box.height)
		);

		Vector distance = center - closestPoint;

		return distance.magnitude() < radius;
	}

	bool collidingWith(Circle circle) {
		return (circle.center - center).magnitude() < circle.radius + radius;
	}
};