#pragma once
#include "Component.h"
#include <vector>
#include <iostream>

struct Collision {

	long sourceUid;

	GameObject* gameObject;

	bool up;
	bool down;
	bool left;
	bool right;

	float upOverlap;
	float downOverlap;
	float leftOverlap;
	float rightOverlap;

	bool isColliding() {
		return up || down || left || right;
	}

	void print() {
		std::cout << "Colliding Up: " << up << "\nColliding Down: " << down << "\nColliding Right: " << right << "\nColliding Left: " << left << "\n\n";
	}

	std::string subString(std::string tag, bool colliding, float offset) {
		return tag + ": " + std::to_string(colliding) +" (" + std::to_string(offset) + ")";
	}

	std::string toString() {
		return subString("Up", up, upOverlap) + " " + subString("Down", down, downOverlap) + " " + subString("Left", left, leftOverlap) + " " + subString("Right", right, rightOverlap);
	}
};

class CollisionComponent : public Component
{
public:
	CollisionComponent();
	~CollisionComponent();

	std::vector<Collision> currentCollisions;

	//virtual std::vector<Collision> checkCollisions(std::vector<GameObject *> gameObjects) = 0;

	bool pollCollisions = false;

	bool isColliding = false;
};

