#pragma once
#include "GameObject.h"
#include "Component.h"
#include "BoxCollider.h"
#include "Scene.h"
#include "Pong_GameLoop.h"

class Pong_BallController : public Component
{
public:
	Pong_BallController(Vector origin);
	~Pong_BallController();

	float initialSpeed = 3;
	float speed = 3;

	Vector velocity = Vector::Zero();
	Vector origin;

	void launch(Vector initialVelocity);

	void update();
};

