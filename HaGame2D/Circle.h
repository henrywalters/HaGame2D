#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include "Box.h"
#include "Line.h"
#include "Vector.h"
#include "ICollidable.hpp"

class Box;
class Line;

class Circle : public ICollidable {
public:
	Vector center;
	double radius;

	bool collidingWithBox(Box box) {
		Vector closestPoint = Vector(
			Math::clamp(center.x, box.x, box.x + box.width),
			Math::clamp(center.y, box.y, box.y + box.height)
		);

		Vector distance = center - closestPoint;

		return distance.magnitude() < radius;
	}

	bool collidingWithCircle(Circle circle) {
		return (circle.center - center).magnitude() < circle.radius + radius;
	}

	bool collidingWithLine()

	CollisionType getCollisionType() {
		return CollisionTypes::CircleCollider;
	}


};

#endif