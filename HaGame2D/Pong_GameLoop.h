#pragma once
#include "GameObject.h"
#include "Component.h"
#include <iostream>

class Pong_GameLoop : public Component
{
public:

	int playerScore;
	int computerScore;

	Pong_GameLoop();
	~Pong_GameLoop();

	void update();

	void playerScored();
	void computerScored();
};

