#include "stdafx.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider(float _width, float _height)
{
	width = _width;
	height = _height;
}


BoxCollider::~BoxCollider()
{
}

std::vector<Collision> BoxCollider::checkCollisions(std::vector<GameObject *> gameObjects) {

	
	Box rect1 = getBox();
	std::vector<Collision> collisions;
	if (active) {
		for (int i = 0; i < gameObjects.size(); i++) {
			if (gameObjects[i]->active) {
				Collision collision = { gameObjects[i], false, false, false, false, 0, 0, 0, 0 };
				BoxCollider *boxColl = gameObjects[i]->getComponent<BoxCollider>();
				if (boxColl != NULL && boxColl->active) {
					Box rect2 = boxColl->getBox();
					if (rect1.x < rect2.x + rect2.width &&
						rect1.x + rect1.width > rect2.x &&
						rect1.y < rect2.y + rect2.height &&
						rect1.y + rect1.height > rect2.y) {
						// collision detected!

						if (rect1.y + rect1.height - epsilon > rect2.y) {
							collision.down = true;
						}

						if (rect1.y + epsilon < rect2.y) {
							collision.up = true;
						}

						if (rect1.x + rect1.width - epsilon > rect2.x) {
							collision.right = true;
						}

						if (rect1.x + epsilon < rect2.x) {
							collision.left = true;
						}

						collisions.push_back(collision);
					}
				}
			}
		}
	}
	currentCollisions = collisions;
	return collisions;
}

bool BoxCollider::checkCollision(Box rect1, Box rect2) {
	if (rect1.x <= rect2.x + rect2.width &&
		rect1.x + rect1.width >= rect2.x &&
		rect1.y <= rect2.y + rect2.height &&
		rect1.y + rect1.height >= rect2.y) {
		// collision detected!
		return true;
	}
	else {
		return false;
	}
}