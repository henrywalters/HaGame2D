#pragma once
#ifndef ICOLLIDABLE
#define ICOLLIDABLE

#include "Line.h"
#include "Box.h"
#include "Circle.h"
#include <string>
#include <exception>

enum CollisionType {
	LineCollider,
	BoxCollider,
	CircleCollider,
	// TODO: support a Polygon
};

class ICollidable {
public:
	virtual CollisionType getCollisionType() = 0;
	virtual bool collidingWithLine(Line line) = 0;
	virtual bool collidingWithBox(Box box) = 0;
	virtual bool collidingWithCircle(Circle circle) = 0;

	virtual Line getLine() {
		throw new std::exception("getLine unimplemented for Line Collider");
	}

	virtual Box getBox() {
		throw new std::exception("getBox unimplemented for Box Collider");
	}

	virtual Circle getCircle() {
		throw new std::exception("getCircle unimplemented for Circle Collider");
	}


	bool collidingWith(ICollidable collider) {
		switch (collider.getCollisionType()) {
		case CollisionType::LineCollider:
			return collidingWithLine(collider.getLine());
		case CollisionType::BoxCollider:
			return collidingWithBox(collider.getBox());
		case CollisionType::CircleCollider:
			return collidingWithCircle(collider.getCircle());
		default:
			throw new std::exception("unimplemented collider type");
		}
	}
};

#endif