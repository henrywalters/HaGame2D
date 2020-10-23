#pragma once
#include "Component.h"
#include <vector>
#include <iostream>
#include "ICollidable.hpp"

struct Collision {

	long sourceUid;

	GameObject* gameObject;

	bool up;
	bool down;
	bool left;
	bool right;

	double upOverlap;
	double downOverlap;
	double leftOverlap;
	double rightOverlap;

	bool isColliding() {
		return up || down || left || right;
	}

	void print() {
		std::cout << "Colliding Up: " << up << "\nColliding Down: " << down << "\nColliding Right: " << right << "\nColliding Left: " << left << "\n\n";
	}

	std::string subString(std::string tag, bool colliding, double offset) {
		return tag + ": " + std::to_string(colliding) +" (" + std::to_string(offset) + ")";
	}

	std::string toString() {
		return subString("Up", up, upOverlap) + " " + subString("Down", down, downOverlap) + " " + subString("Left", left, leftOverlap) + " " + subString("Right", right, rightOverlap);
	}
};

class CollisionComponent : public Component
{
public:
	std::vector<Collision> currentCollisions;
	bool pollCollisions = false;
	bool isColliding = false;

	CollisionComponent();
	~CollisionComponent();

	virtual ICollidable getCollider() = 0;
};

