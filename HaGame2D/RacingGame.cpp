#include "stdafx.h"
#include "RacingGame.h"
#include "CarController.h"

const int CAR_WIDTH = 34;
const int CAR_HEIGHT = 56;
const char *CAR_SPRITE = "../Assets/Sprites/HaGameEngine/Misc/car.png";

class Car {
	int width = CAR_WIDTH;
	int height = CAR_HEIGHT;
	GameObject* car;

public:
	Car(Scene * scene, Vector startPos) {
		car = scene->add();
		car->addComponent(new SpriteRenderer(CAR_SPRITE, width, height, NULL));
		car->addComponent(new CarController());
		car->setPosition(startPos);
		car->origin = Vector(width / 2, height / 2);
		car->z_index = 10;
	}
};

RacingGame::RacingGame()
{
	Game game = Game(800, 600, "Racing Game");

	Scene* track = game.addScene("track");
	track->setDisplayPort(0, 0, 700, 600);

	Car car = Car(track, Vector(350, 300));

	game.prepareScene();

	while (game.running) {
		game.tick();
	}
}

RacingGame::~RacingGame()
{
}
