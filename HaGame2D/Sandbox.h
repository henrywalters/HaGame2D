#pragma once
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
#include "CollisionSystem.h"

void sandbox() {
	Game game = Game(600, 600, "Experiments");

	EventManager<Vector> events = EventManager<Vector>();

	Scene* sandbox = game.addScene("sandbox");
	sandbox->setDisplayPort(0, 0, 600, 600);
	sandbox->setLogger(new Logger());

	GameObject* player = sandbox->add()
		->setPosition(Vector(50, 50));

	auto collider = player->addComponent(new CircleCollider(50));
	auto renderer = player->addComponent(new SpriteRenderer("../Assets/Sprites/HaGameEngine/Characters/Kitty.png", 50, 50, NULL));

	collider->pollCollisions = true;

	int playerSpeed = 20;

	sandbox->addSystem(new CollisionSystem());

	sandbox->initialize();

	while (game.running) {

		auto objects = sandbox->getGameObjectsWhere<CircleRenderer>();

		player->rotate(sandbox->dt_s() * 1);

		if (sandbox->input->fire1Down) {
			events.emit("fire", sandbox->input->mousePos());
			auto cube = sandbox->instantiate((new GameObject())
				->setPosition(sandbox->input->mousePos())
				->addComponentAnd(new CircleRenderer(50))
				->addComponentAnd(new CircleCollider(50))
			);

			cube->getComponent<CollisionComponent>()->pollCollisions = true;
		}

		if (sandbox->input->fire2) {
			auto cube = sandbox->instantiate((new GameObject())
				->setPosition(sandbox->input->mousePos() - Vector(22.5, 22.5))
				->addComponentAnd(new BoxRenderer(50, 50))
				->addComponentAnd(new BoxCollider(50, 50))
			);

			
		}

		player->setPosition(sandbox->input->mousePos());

		if (sandbox->input->shift) {
			playerSpeed = 20;
		}
		else {
			playerSpeed = 5;
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

}