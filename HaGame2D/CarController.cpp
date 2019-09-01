#include "stdafx.h"
#include "CarController.h"
#include "Scene.h"

void CarController::handleInput()
{
	acceleration = Vector::Zero();

	float speed = velocity.magnitude();

	if (input->down && (speed <= props.maxSpeed || velocity.y <= 0)) {
		acceleration.y += props.acceleration;
	}

	if (input->up && (speed <= props.maxSpeed || velocity.y >= 0)) {
		acceleration.y += -1 * props.acceleration;
	}

	if (input->right) {
		transform->rotate(props.turningSensitivity * (speed / props.maxSpeed));
	}

	if (input->left) {
		transform->rotate(props.turningSensitivity * -1 * (speed / props.maxSpeed));
	}

	if (input->space && velocity.magnitude() > 0) {
		acceleration.y += velocity.y < 0 ? props.breakDeacceleration : -1 * props.breakDeacceleration;
	}
}

void CarController::handlePhysics()
{
	velocity.y += gameScene->dt() * acceleration.y;
	log(std::to_string(velocity.magnitude()) + "mph");
}

void CarController::update()
{
	handleInput();
	handlePhysics();
	log(velocity.toString());
	transform->move(velocity);

	Vector tires[4] = {
		transform->position + Vector(0, 0),
		transform->position + Vector(0, 34),
		transform->position + Vector(56, 0),
		transform->position + Vector(56, 34),
	};

	for (int i = 0; i < 4; i++) {
		GameObject* mark = new GameObject();
		mark->setPosition(tires[i])->setZIndex(5)->addComponent(new BoxRenderer(5, 5, true, Color::blue()));
		gameScene->instantiate(mark);
	}
}
