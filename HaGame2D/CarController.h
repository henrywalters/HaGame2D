#pragma once
#include "Component.h"

const float DEFAULT_ACCELERATION = 0.001;
const float DEFAULT_BREAKING_DEACCEL = .1;
const float DEFAULT_TURNING_SENSITIVITY = .1;
const float DEFAULT_MAX_SPEED = 4;

struct CarProperties {
	float acceleration;
	float turningSensitivity;
	float breakDeacceleration;
	float maxSpeed;
};

class CarController : public Component
{
	Vector acceleration = Vector::Zero();
	Vector velocity = Vector::Zero();

	CarProperties props;

	void handleInput();
	void handlePhysics();

public:

	CarController(CarProperties properties) {
		props = properties;
	}

	CarController() {
		props.acceleration = DEFAULT_ACCELERATION;
		props.breakDeacceleration = DEFAULT_BREAKING_DEACCEL;
		props.turningSensitivity = DEFAULT_TURNING_SENSITIVITY;
		props.maxSpeed = DEFAULT_MAX_SPEED;
	}

	void update();
};

