#pragma once
#include "GameObject.h"
#include "CollisionComponent.h"
#include <vector>
#include <string>

struct Box {
	float x;
	float y;
	float width;
	float height;

	bool boundedBy(Box otherBox) {
		return (x >= otherBox.x && x + width <= otherBox.x + otherBox.width && y >= otherBox.y && y + height <= otherBox.y + otherBox.height);
	}

	bool collidingWith(Box rect) {
		return (x <= rect.x + rect.width &&
			x + width >= rect.x &&
			y <= rect.y + rect.height &&
			y + height >= rect.y);
	}

	SDL_Rect * getSDLRectPointer() {
		SDL_Rect * rect;
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
};

class BoxCollider : public CollisionComponent
{

	const float epsilon = 5;

public:

	float width;
	float height;

	BoxCollider(float width, float height);
	~BoxCollider();

	Box getBox() {
		Box box = { transform->position.x, transform->position.y, width, height };
		return box;
	}

	std::vector<Collision> checkCollisions(std::vector<GameObject *> gameObjects);

	static bool checkCollision(Box box1, Box box2);
};

