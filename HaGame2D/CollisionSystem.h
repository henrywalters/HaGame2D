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
#include "GameObjectGrid.h"

const Vector COLLISION_GRID_CELL_SIZE = Vector(800, 800);

class CollisionSystem :
	public System
{

public:

	EventChannel<Collision> events;
	GameObjectGrid grid = GameObjectGrid(Vector::Zero(), COLLISION_GRID_CELL_SIZE);

	CollisionSystem() : System("Collision_System") {
		events = EventChannel<Collision>();
	}

	void update() {
		auto gameObjects = getScene()->getGameObjectsWhere<CollisionComponent>();
		int checks = 0;

		grid.addMany(gameObjects);

		for (auto object : gameObjects) {
			auto collider = object->getComponent<CollisionComponent>();
			
			if (collider->pollCollisions && collider->active) {
				checks++;
				collider->isColliding = false;

				BoxCollider* boxCollider = object->getComponent<BoxCollider>();
				if (boxCollider) collider->currentCollisions = checkBoxCollisions(object->uid, boxCollider->getBox(), grid.getNeighbors(object));

				CircleCollider* circleCollider = object->getComponent<CircleCollider>();
				if (circleCollider) collider->currentCollisions = checkCircleCollisions(object->uid, circleCollider->getCircle(), grid.getNeighbors(object));

				if (collider->currentCollisions.size() > 0) {
					collider->isColliding = true;
					for (auto collision : collider->currentCollisions) {
						events.trigger(collision);
					}
				}
			}
		}

		grid.clear();
	}

	std::vector<Collision> checkCircleCollisions(long uid, Circle circle, std::vector<GameObject*> gameObjects) {
		auto collisions = std::vector<Collision>();

		for (auto object : gameObjects) {

			if (object->uid != uid) {
				bool colliding = false;

				BoxCollider* boxCollider = object->getComponent<BoxCollider>();
				if (boxCollider && boxCollider->active && circle.collidingWithBox(boxCollider->getBox())) colliding = true;

				CircleCollider* circleCollider = object->getComponent<CircleCollider>();
				if (circleCollider && circleCollider->active && circle.collidingWith(circleCollider->getCircle())) colliding = true;

				if (colliding) {
					Collision collision;
					collision.sourceUid = uid;
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
					collision.sourceUid = uid;
					collision.gameObject = object;
					collisions.push_back(collision);
				}
			}
		}

		return collisions;
	}
};

