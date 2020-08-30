#pragma once
#include "System.h"
#include "QuadTree.h"
#include "CollisionComponent.h"
#include "events.h"
#include <iostream>
#include "Circle.h"
#include "Box.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

class CollisionSystem :
	public System
{

public:

	EventChannel<Collision> events;

	CollisionSystem() : System("Collision_System") {
		events = EventChannel<Collision>();
	}

	void update() {
		auto gameObjects = getScene()->getGameObjectsWhere<CollisionComponent>();
		for (auto object : gameObjects) {
			auto collider = object->getComponent<CollisionComponent>();
			
			if (collider->pollCollisions) {
				collider->isColliding = false;

				BoxCollider* boxCollider = object->getComponent<BoxCollider>();
				if (boxCollider) collider->currentCollisions = checkBoxCollisions(object->uid, boxCollider->getBox(), gameObjects);

				CircleCollider* circleCollider = object->getComponent<CircleCollider>();
				if (circleCollider) collider->currentCollisions = checkCircleCollisions(object->uid, circleCollider->getCircle(), gameObjects);

				if (collider->currentCollisions.size() > 0) {
					collider->isColliding = true;
					for (auto collision : collider->currentCollisions) {
						events.trigger(collision);
					}
				}
			}
		}
	}

	std::vector<Collision> checkCircleCollisions(long uid, Circle circle, std::vector<GameObject*> gameObjects) {
		auto collisions = std::vector<Collision>();

		for (auto object : gameObjects) {

			if (object->uid != uid) {
				bool colliding = false;

				BoxCollider* boxCollider = object->getComponent<BoxCollider>();
				if (boxCollider && circle.collidingWithBox(boxCollider->getBox())) colliding = true;

				CircleCollider* circleCollider = object->getComponent<CircleCollider>();
				if (circleCollider && circle.collidingWith(circleCollider->getCircle())) colliding = true;

				if (colliding) {
					Collision collision;
					collision.gameObject = object;
					collisions.push_back(collision);
				}
			}
		}

		return collisions;
	}

	std::vector<Collision> checkBoxCollisions(long uid, Box box, std::vector<GameObject*> gameObjects) {

		auto collisions = std::vector<Collision>();

		for (auto object : gameObjects) {
			if (object->uid != uid) {
				bool colliding = false;

				BoxCollider* boxCollider = object->getComponent<BoxCollider>();
				if (boxCollider && box.collidingWith(boxCollider->getBox())) colliding = true;

				CircleCollider* circleCollider = object->getComponent<CircleCollider>();
				if (circleCollider && box.collidingWithCircle(circleCollider->getCircle())) colliding = true;

				if (colliding) {
					Collision collision;
					collision.gameObject = object;
					collisions.push_back(collision);
				}
			}
		}

		return collisions;
	}
};

