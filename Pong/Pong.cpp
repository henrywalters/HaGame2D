// Pong.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HaGame2D.h"

int main(int argc, char* argv[])
{
	const int paddleWidth = 25;
	const int paddleHeight = 100;
	const int ballSize = 25;

	Scene pong = Scene();
	pong.title = "Pong";

	pong.initiailize();

	auto grid = pong.add();

	auto player = pong.add();
	auto computer = pong.add();
	auto ball = pong.add();

	GridLines * gridlines = grid->addComponent(new GridLines(20, 20));

	BoxRenderer * playerRenderer = player->addComponent(new BoxRenderer(paddleWidth, paddleHeight));
	BoxRenderer * compRenderer = computer->addComponent(new BoxRenderer(paddleWidth, paddleHeight));
	BoxRenderer * ballRenderer = ball->addComponent(new BoxRenderer(ballSize, ballSize));

	OrthogonolCharacterController * playerController = player->addComponent(new OrthogonolCharacterController());

	playerController->speed = 4;
	playerController->xAxisLocked = true;

	playerRenderer->fill = true;
	compRenderer->fill = true;
	ballRenderer->fill = true;

	playerRenderer->color = Color::blue();
	compRenderer->color = Color::red();

	player->move(Vector(50, pong.screenHeight / 2 - paddleHeight / 2));
	computer->move(Vector(pong.screenWidth - paddleWidth - 50, pong.screenHeight / 2 - paddleHeight / 2));
	ball->move(Vector(pong.screenWidth / 2 - ballSize / 2, pong.screenHeight / 2 - ballSize / 2));

	pong.initializeGameObjects();

	pong.loop();
    return 0;
}

