#pragma once
#include "SDL.h"
#include "Vector.h"
#include "Math.h"
#include "Algorithms.h"
#include "ICollidable.hpp"

#ifndef BOX_H
#define BOX_H

class Circle;
class Line;

class Box : public ICollidable {
public:

	double x;
	double y;
	double width;
	double height;

	bool boundedBy(Box otherBox) {
		return (x >= otherBox.x && x + width <= otherBox.x + otherBox.width && y >= otherBox.y && y + height <= otherBox.y + otherBox.height);
	}

	bool containsPoint(Vector point) {
		return x <= point.x && x + width >= point.x && y <= point.y && y + height >= point.y;
	}

	bool collidingWithBox(Box rect) {
		return (x <= rect.x + rect.width &&
			x + width >= rect.x &&
			y <= rect.y + rect.height &&
			y + height >= rect.y);
	}

	bool collidingWithCircle(Circle circle);

	bool collidingWithLine(Line line);

	CollisionType getCollisionType() {
		return CollisionType::BoxCollider;
	}

	Box getBox() {
		return *this;
	}

	SDL_Rect* getSDLRectPointer() {
		SDL_Rect* rect;
		rect->x = x;
		rect->y = y;
		rect->w = width;
		rect->h = height;
		return rect;
	}

	SDL_Rect getSDLRect() {
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = width;
		rect.h = height;
		return rect;
	}

	static Box empty() {
		Box newBox = Box{ 0, 0, 0, 0 };
		return newBox;
	}

	std::string toString() {
		return "x: " + std::to_string(x) + " Y: " + std::to_string(y) + " W: " + std::to_string(width) + " H: " + std::to_string(height);
	}

	bool operator == (const Box& box) {
		return (x == box.x && y == box.y && width == box.width && height == box.height);
	}

	bool xOverlaps(Box box) {
		return (x > box.x && x < box.x + box.width) || (x + width > box.x && x + width < box.x + box.width);
	}

	bool yOverlaps(Box box) {
		return (y > box.y && x < box.y + box.height) || (y + height > box.y && y + height < box.y + box.height);
	}

	Vector position() {
		return Vector(x, y);
	}

	Vector size() {
		return Vector(width, height);
	}
};

#endif