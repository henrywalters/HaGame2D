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
			if (gameObjects[i]->active && gameObjects[i]->uid != transform->uid) {
				Collision collision = { gameObjects[i], false, false, false, false, 0, 0, 0, 0 };
				BoxCollider *boxColl = gameObjects[i]->getComponent<BoxCollider>();
				if (boxColl != NULL && boxColl->active) {
					Box rect2 = boxColl->getBox();
					
					bool colliding = false;

					// Downwards collision
					if (rect1.y + rect1.height + epsilon >= rect2.y && rect1.xOverlaps(rect2)) {
						collision.down = true;
						collision.downOverlap = rect1.y + rect1.height - rect2.y;
						colliding = true;
					}

					if (rect1.y - epsilon <= rect2.y + rect2.height && rect1.xOverlaps(rect2)) {
						collision.up = true;
						collision.upOverlap = rect1.y - rect2.y - rect2.height;
						colliding = true;
					}

					if (rect1.x + rect1.width + epsilon >= rect2.x && rect1.yOverlaps(rect2)) {
						collision.right = true;
						collision.rightOverlap = rect1.x + rect1.width - rect2.x;
						colliding = true;
					}

					if (rect1.x - epsilon <= rect2.x + rect2.width && rect1.yOverlaps(rect2)) {
						collision.left = true;
						collision.leftOverlap = rect1.x - rect2.x - rect2.width;
						colliding = true;
					}

					if (colliding) {
						collisions.push_back(collision);
					}

					/*if (rect1.x < rect2.x + rect2.width &&
						rect1.x + rect1.width > rect2.x &&
						rect1.y < rect2.y + rect2.height &&
						rect1.y + rect1.height > rect2.y) {
						// collision detected!

						collision.downOverlap = 0;
						collision.upOverlap = 0;
						collision.leftOverlap = 0;
						collision.rightOverlap = 0;

						if (rect1.y + rect1.height + epsilon > rect2.y) {
							collision.down = true;
							collision.downOverlap = rect1.y + rect1.height - rect2.y;
						}

						if (rect1.y - epsilon < rect2.y + rect2.height) {
							collision.up = true;
							collision.upOverlap = rect1.y - rect2.y - rect2.height;
						}

						if (rect1.x + rect1.width + epsilon > rect2.x) {
							collision.right = true;
							collision.rightOverlap = rect1.x + rect1.width - rect2.x;
						}

						if (rect1.x - epsilon < rect2.x + rect2.width) {
							collision.left = true;
							collision.leftOverlap = rect1.x - rect2.x - rect2.width;
						}

						collisions.push_back(collision);
					}
					*/
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