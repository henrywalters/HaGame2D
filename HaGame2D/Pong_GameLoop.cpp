#include "stdafx.h"
#include "Pong_GameLoop.h"


Pong_GameLoop::Pong_GameLoop()
{
	playerScore = 0;
	computerScore = 0;
}


Pong_GameLoop::~Pong_GameLoop()
{
}

void Pong_GameLoop::update() {

}

void Pong_GameLoop::playerScored() {
	playerScore += 1;
}

void Pong_GameLoop::computerScored() {
	computerScore += 1;
}