#pragma once
#include "GameObject.h"
#include "Component.h"
#include "BoxCollider.h"
#include "Vector.h"
#include "InputComponent.h"

class OrthogonolCharacterController: public Component
{
public:
	OrthogonolCharacterController();
	~OrthogonolCharacterController();

	float speed = 2.0f;

	Vector currentDirection = Vector::Zero();

	bool xAxisLocked = false;
	bool yAxisLocked = false;

	bool positionedCamera = false;

	void onCreate() {
		transform->addTag(IGNORE_ORIGIN);
	}

	void update();
};

