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
		->setPosition(Vector(50, 50))
		->addComponentAnd(new CircleRenderer(50));

	

	GameObject* container = player->add();
	container->move(Vector(-50, -50));
	container->addComponentAnd(new BoxRenderer(100, 100))
		->addComponentAnd(new BoxCollider(100, 100));

	BoxCollider* collider = container->getComponent<BoxCollider>();
	collider->pollCollisions = true;
	auto renderer = container->getComponent<BoxRenderer>();

	int playerSpeed = 20;

	sandbox->addSystem(new CollisionSystem());

	sandbox->initialize();

	while (game.running) {

		auto objects = sandbox->getGameObjectsWhere<CircleRenderer>();

		if (collider->collidingWith.size() > 0) {
			renderer->color = Color::green();
		}
		else {
			renderer->color = Color::blue();
		}

		if (sandbox->input->fire1) {
			events.emit("fire", sandbox->input->mousePos());
			auto cube = sandbox->instantiate((new GameObject())
				->setPosition(sandbox->input->mousePos() - Vector(2.5, 2.5))
				->addComponentAnd((new BoxCollider(5, 5)))
				->addComponentAnd(new BoxRenderer(5, 5))
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
