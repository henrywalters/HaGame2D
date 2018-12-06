#pragma once
#include "GameObject.h"
#include "Component.h"
#include "BoxCollider.h"

class Pong_ComputerController : public Component
{
public:

	GameObject * ball;

	float speed = 5;

	Pong_ComputerController(GameObject * ball);
	~Pong_ComputerController();

	void update();
};

