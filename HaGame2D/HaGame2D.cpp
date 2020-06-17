// HaGame2D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDL.h"
#include "HaGame2D.h"
#include <iostream>
#include "ZeldaClone.h"
#include "SpriteSheetHelper.h"
#include "MapBuilder.h"
#include "MapLoader.h"
#include "BlackJack.h"
#include "CasinoAdventure.h"
#include "Platformer.h"
#include "HaGame3D.h"
#include "Snake.h"
#include "DoublyLinkedList.h"
#include "CircleRenderer.h"

int main(int argc, char* argv[])
{
	Game game = Game(600, 600, "Experiments");

	Scene* sandbox = game.addScene("sandbox");
	sandbox->setDisplayPort(0, 0, 600, 600);
	sandbox->setLogger(new Logger());

	GameObject* player = sandbox->add()
		->setPosition(Vector(50, 50))
		->addComponentAnd(new CircleRenderer(50));

	sandbox->initializeGameObjects();

	int playerSpeed = 5;

	while (game.running) {

		if (sandbox->input->fire1Down) {
			sandbox->logger->log("Fire");

			sandbox->instantiate((new GameObject())
				->setPosition(sandbox->input->mousePos() - Vector(2.5, 2.5))
				->addComponentAnd(new CircleRenderer(10))
			);
		}

		if (sandbox->input->up) {
			player->move(Vector(0, -playerSpeed * sandbox->dt_s()));
		}

		if (sandbox->input->down) {
			player->move(Vector(0, playerSpeed * sandbox->dt_s()));
		}

		if (sandbox->input->right) {
			player->move(Vector(playerSpeed * sandbox->dt_s(), 0));
		}

		if (sandbox->input->left) {
			player->move(Vector(-playerSpeed * sandbox->dt_s(), 0));
		}

		game.tick();
	}

	return 0;
}
