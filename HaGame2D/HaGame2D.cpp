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
#include "CollisionSystem.h"

GameObject* newRect(Scene *scene, Vector pos, Vector size) {
	return scene->add()
		->setPosition(pos)
		->addComponentAnd(new BoxRenderer(size.x, size.y))
		->addComponentAnd(new BoxCollider(size.x, size.y));
}

int main(int argc, char* argv[])
{
	Game game = Game(600, 600, "Experiments");

	EventManager<Vector> events = EventManager<Vector>();

	Scene* sandbox = game.addScene("sandbox");
	sandbox->setDisplayPort(0, 0, 600, 600);
	sandbox->setLogger(new Logger());

	GameObject* player = sandbox->add()
		->setPosition(Vector(50, 50));

	auto collider = player->addComponent(new CircleCollider(50));
	auto renderer = player->addComponent(new CircleRenderer(50));

	collider->pollCollisions = true;

	int playerSpeed = 20;

	sandbox->addSystem(new CollisionSystem());

	sandbox->initialize();

	while (game.running) {

		auto objects = sandbox->getGameObjectsWhere<CircleRenderer>();

		if (collider->isColliding) {
			renderer->color = Color::green();
		}
		else {
			renderer->color = Color::blue();
		}

		if (sandbox->input->fire1Down) {
			events.emit("fire", sandbox->input->mousePos());
			auto cube = sandbox->instantiate((new GameObject())
				->setPosition(sandbox->input->mousePos())
				->addComponentAnd(new CircleRenderer(50))
				->addComponentAnd(new CircleCollider(50))
			);
		}

		if (sandbox->input->fire2) {
			auto cube = sandbox->instantiate((new GameObject())
				->setPosition(sandbox->input->mousePos() - Vector(10, 10))
				->addComponentAnd(new BoxRenderer(20, 20))
				->addComponentAnd(new BoxCollider(20, 20))
			);
		}

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

	return 0;
}
