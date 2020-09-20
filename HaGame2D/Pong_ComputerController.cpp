#include "stdafx.h"
#include "Pong_ComputerController.h"


Pong_ComputerController::Pong_ComputerController(GameObject * _ball)
{
	ball = _ball;
}


Pong_ComputerController::~Pong_ComputerController()
{
}

void Pong_ComputerController::update() {
	BoxCollider * collider = ball->getComponent<BoxCollider>();
	Box ballBox = collider->getBox();
	BoxCollider * paddleCollider = transform->getComponent<BoxCollider>();
	Box paddle = paddleCollider->getBox();

	double paddleCenter = paddle.y + (paddle.height / 2);
	double ballCenter = ballBox.y + (ballBox.height / 2);

	if (paddleCenter > ballCenter) {
		transform->move(Vector(0, -1 * speed));
	}
	if (paddleCenter < ballCenter) {
		transform->move(Vector(0, speed));
	}
}