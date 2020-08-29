#pragma once
#include "HaGame2D.h"
#include "TGMPlayerController.h"

class OLC_2020_TheGreatMachine : public Game
{
private:

	Scene* game;
	Scene* ui;

	GameObject* player;

public:
	OLC_2020_TheGreatMachine() : Game(800, 800, "The Great Machine") {
		game = addScene("game");
		ui = addScene("ui");

		player = game->add()
			->addComponentAnd(new SpriteRenderer("../Assets/Sprites/HaGameEngine/Characters/Kitty.png", 50, 50, NULL))
			->addComponentAnd(new TGM::BodyComponent(100))
			->addComponentAnd<CircleCollider>(new CircleCollider(25, false))
			->addComponentAnd<CircleRenderer>(new CircleRenderer(25, false));

		player->origin = Vector(25, 25);

		player->setPosition(Vector(400, 400));

		game->addSystem(new TGM::PlayerController(player));
	};

	void run() {
		game->initialize();
		ui->initialize();

		while (running) {
			tick();
		}
	}
};

