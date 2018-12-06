#include "stdafx.h"
#include "Pong_BallController.h"

Pong_BallController::Pong_BallController(Vector _origin)
{
	origin = _origin;
}


Pong_BallController::~Pong_BallController()
{
}

void Pong_BallController::update() {
	BoxCollider *collider = transform->getComponent<BoxCollider>();

	std::vector<Collision> collisions = collider->currentCollisions;

	for (int i = 0; i < collisions.size(); i++) {
		GameObject * collidedWith = collisions[i].gameObject;

		if (collidedWith->tag == "right wall") {
			Pong_GameLoop *game = Scene::getGameObject("pong")->getComponent<Pong_GameLoop>();
			game->playerScored();
			velocity.x *= -1;
			//speed = initialSpeed;
			//transform->setPosition(origin);
			//launch(Vector(-2, 1));
		}

		if (collidedWith->tag == "left wall") {
			Pong_GameLoop *game = Scene::getGameObject("pong")->getComponent<Pong_GameLoop>();
			game->computerScored();
			velocity.x *= -1;
			//speed = initialSpeed;
			//transform->setPosition(origin);
			//launch(Vector(2, 1));
		}

		if (collidedWith->tag == "vertical wall") {
			velocity.y *= -1;
		}

		if (collidedWith->tag == "paddle") {
			//velocity.x *= -1;
			//speed += .5;
		}


	}

	transform->move(velocity * speed);
}

void Pong_BallController::launch(Vector initialVelocity) {
	velocity = initialVelocity.normalized() * speed;
}