#include "stdafx.h"
#include "OrthogonolCharacterController.h"
#include <iostream>
#include "BoxRenderer.h"
#include <math.h>
#include "Scene.h"

OrthogonolCharacterController::OrthogonolCharacterController()
{
}


OrthogonolCharacterController::~OrthogonolCharacterController()
{
}

void OrthogonolCharacterController::update() {

	BoxCollider *collider = transform->getComponent<BoxCollider>();

	bool undoX = false, undoY = false;

	Vector vel = Vector::Zero();

	float epsilon = 3;
	Vector tmpDir = Vector::Zero();
	bool moved = false;
	if (!xAxisLocked) {
		if (input->right) {
			vel.x = speed;
			tmpDir += Vector(1, 0);
			moved = true;
		}

		if (input->left) {
			vel.x = -1 * speed;
			tmpDir += Vector(-1, 0);
			moved = true;
		}
	}

	if (!yAxisLocked) {
		if (input->up) {
			vel.y = -1 * speed;
			tmpDir += Vector(0, -1);
			moved = true;
		}
		else if (input->down) {
			vel.y = speed;
			tmpDir += Vector(0, 1);
			moved = true;
		}
	}

	if (moved) {
		currentDirection = tmpDir;
	}

	Vector finalVelocity = vel.normalized() * speed;

	if (collider != NULL) {

		std::vector<Collision> collisions;

		if (collisions.size() >= 0) {

			Vector postCollisionVel = finalVelocity;

			transform->move(Vector(finalVelocity.x, 0));
			collisions = collider->currentCollisions;

			if (abs(vel.x) != 0) {
				for (auto collision : collisions) {
					GameObject * object = collision.gameObject;
					if (object != NULL) {
						if (object->tag == "wall") {
							BoxCollider * otherCollider = object->getComponent<BoxCollider>();

							// Check if colliding in the right direction
							if ((vel.x > 0 && transform->position.x < object->position.x) || (vel.x < 0 && transform->position.x + collider->width > object->position.x + otherCollider->height)) {
								// Check for false positives with an epsilon value
								if (
									(transform->position.y + collider->height > object->position.y + epsilon && transform->position.y < object->position.y) 
									|| 
									(transform->position.y < object->position.y + otherCollider->height - epsilon && transform->position.y + collider->height > object->position.y + otherCollider->height)
									) 
								{
									undoX = true;
								}
							}	
						}
					}
				}
			}

			if (undoX) {
				transform->move(Vector(finalVelocity.x * -1, 0));
				postCollisionVel.x = 0;
			}
			else {
				gameScene->camera.move(Vector(finalVelocity.x, 0));
			}

			transform->move(Vector(0, finalVelocity.y));
			collisions = collider->currentCollisions;

			if (abs(vel.y) != 0) {
				for (auto collision : collisions) {
					GameObject * object = collision.gameObject;			
					if (object != NULL) {
						if (object->tag == "wall") {
							BoxCollider * otherCollider = object->getComponent<BoxCollider>();
							if ((vel.y > 0 && transform->position.y < object->position.y) || (vel.y < 0 && transform->position.y + collider->height > object->position.y + otherCollider->height)) {
								if (
									(transform->position.x + collider->width > object->position.x + epsilon && transform->position.x < object->position.x) 
									|| 
									(transform->position.x < object->position.x + otherCollider->width - epsilon && transform->position.x + collider->width > object->position.x + otherCollider->width)
								) {
									undoY = true;
								}							
							}
						}
					}
				}
			}

			if (undoY) {
				transform->move(Vector(0, finalVelocity.y * -1));
				postCollisionVel.y = 0;
			}
			else {
				gameScene->camera.move(Vector(0, finalVelocity.y));
			}
		}
		else {
			transform->move(finalVelocity);
			
		}

	}
	else {
		transform->move(finalVelocity);
		gameScene->camera.move(finalVelocity);
	}



	//gameScene->origin = transform->position - Vector(gameScene->screenWidth / 2, gameScene->screenHeight/2);

}