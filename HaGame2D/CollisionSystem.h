#pragma once
#include "System.h"
#include "QuadTree.h"

#include <iostream> 
class CollisionSystem :
	public System
{

private:

public:

	CollisionSystem() : System("Collision_System") {}

	void update() {
		auto gameObjects = getScene()->getGameObjectsWhere<CollisionComponent>();
		for (auto object : gameObjects) {

			auto collider = object->getComponent<BoxCollider>();

			if (collider->pollCollisions) {
				collider->collidingWith = std::vector<GameObject*>();

				for (auto otherObject : gameObjects) {

					if (object->uid != otherObject->uid) {
						auto otherCollider = otherObject->getComponent<BoxCollider>();

						if (collider && otherCollider) {
							auto b1 = collider->getBox();
							auto b2 = otherCollider->getBox();

							if (b1.collidingWith(b2)) {
								collider->collidingWith.push_back(otherObject);
							}
						}
					}
				}
			}
		}
	}
};

