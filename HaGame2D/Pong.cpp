#include "stdafx.h"
#include "Pong.h"
#include "HaGame2D.h"
#include "Pong_BallController.h"
#include "Pong_ComputerController.h"
#include "Pong_GameLoop.h"
#include <math.h>
#include "Game.h"

char * PLAYER_ID = "player";
char * COMPUTER_ID = "computer";
char * BALL_ID = "ball";
char * PONG = "pong";

Pong::Pong()
{
	const int paddleWidth = 25;
	const int paddleHeight = 100;
	const int ballSize = 15;

	Game game;

	Scene pong = *game.addScene("pong");


	//Initialize all the game objects

	auto grid = pong.add();

	auto player = pong.add();
	player->id = PLAYER_ID;
	auto computer = pong.add();
	computer->id = COMPUTER_ID;
	auto ball = pong.add();
	ball->id = BALL_ID;
	auto walls = pong.add();

	auto leftWall = walls->add();
	auto rightWall = walls->add();
	auto topWall = walls->add();
	auto bottomWall = walls->add();

	auto gameLoop = Scene::instantiate(PONG, new GameObject());
	gameLoop->addComponent(new Pong_GameLoop());

	auto test = new GameObject();
	test->tag = "Testing";

	leftWall->tag = "left wall";
	rightWall->tag = "right wall";
	topWall->tag = "vertical wall";
	bottomWall->tag = "vertical wall";

	rightWall->move(Vector(pong.screenWidth - 10, 0));
	bottomWall->move(Vector(0, pong.screenHeight - 10));

	//Add components for the environment

	//GridLines * gridlines = grid->addComponent(new GridLines(1, 1));

	leftWall->addComponent(new BoxCollider(10, pong.screenHeight));
	rightWall->addComponent(new BoxCollider(10, pong.screenHeight));
	topWall->addComponent(new BoxCollider(pong.screenWidth, 10));
	bottomWall->addComponent(new BoxCollider(pong.screenWidth, 10));

	leftWall->addComponent(new BoxRenderer(10, pong.screenHeight));
	rightWall->addComponent(new BoxRenderer(10, pong.screenHeight));
	topWall->addComponent(new BoxRenderer(pong.screenWidth, 10));
	bottomWall->addComponent(new BoxRenderer(pong.screenWidth, 10));


	//Add components for the actors

	BoxRenderer * playerRenderer = player->addComponent(new BoxRenderer(paddleWidth, paddleHeight));
	BoxRenderer * compRenderer = computer->addComponent(new BoxRenderer(paddleWidth, paddleHeight));
	BoxRenderer * ballRenderer = ball->addComponent(new BoxRenderer(ballSize, ballSize));

	player->addComponent(new BoxCollider(paddleWidth, paddleHeight));
	computer->addComponent(new BoxCollider(paddleWidth, paddleHeight));
	ball->addComponent(new BoxCollider(ballSize, ballSize));

	OrthogonolCharacterController * playerController = player->addComponent(new OrthogonolCharacterController());

	computer->addComponent(new Pong_ComputerController(ball));

	Pong_BallController * ballController = ball->addComponent(new Pong_BallController(Vector(pong.screenWidth / 2 - ballSize / 2, pong.screenHeight / 2 - ballSize / 2)));

	//collision detection testing

	pong.FPS = 60;

	int balls = 150;
	
	for (int i = 0; i < balls; i++) {
		auto b = pong.add();
		b->addComponent(new BoxRenderer(ballSize, ballSize));
		b->tag = "ball";
		b->addComponent(new BoxCollider(ballSize, ballSize));
		auto controller = b->addComponent(new Pong_BallController(Vector(pong.screenWidth / 2 - ballSize / 2, pong.screenHeight / 2 - ballSize / 2)));
		b->move(Vector(pong.screenWidth / 2 - ballSize / 2, pong.screenHeight / 2 - ballSize / 2));
		int min = -100;
		int max = 100;

		int randNum = rand() % (max - min + 1) + min;
		int randNum2 = rand() % (max - min + 1) + min;
		int x = rand() % (pong.screenWidth - 25) + 25;
		int y = rand() % (pong.screenHeight - 25) + 25;
		controller->speed = 3;
		//b->setPosition(Vector(x, y));
		controller->launch(Vector(randNum, randNum2));
	}
	

	//Adjust actor settings

	player->move(Vector(50, pong.screenHeight / 2 - paddleHeight / 2));
	computer->move(Vector(pong.screenWidth - paddleWidth - 50, pong.screenHeight / 2 - paddleHeight / 2));
	ball->move(Vector(pong.screenWidth / 2 - ballSize / 2, pong.screenHeight / 2 - ballSize / 2));

	player->tag = "paddle";
	computer->tag = "paddle";
	//player->active = false;
	//computer->active = false;
	//Adjust actor component settings

	playerController->xAxisLocked = true;

	playerRenderer->fill = true;
	compRenderer->fill = true;
	ballRenderer->fill = true;
	ballController->speed = 2;
	playerRenderer->color = Color::blue();
	compRenderer->color = Color::red();

	//Initialize and loop

	ballController->launch(Vector(2, 1));

	pong.initializeGameObjects();

	pong.loop();
}


Pong::~Pong()
{
}
