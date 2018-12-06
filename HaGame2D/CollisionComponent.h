#pragma once
#include "Component.h"
#include <vector>
#include <iostream>

struct Collision {

	GameObject * gameObject;

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
};

class CollisionComponent : public Component
{
public:
	CollisionComponent();
	~CollisionComponent();

	std::vector<Collision> currentCollisions;

	virtual std::vector<Collision> checkCollisions(std::vector<GameObject *> gameObjects) = 0;
};

