#pragma once
#include "HaGame2D.h"

class InputDemo : public Game {
public:

	Scene* demo;

	GameObject* lAxis;
	GameObject* rAxis;

	GameObject* a, *b, *x, *y;
	GameObject* lT, * rT, * rS, * lS;
	GameObject* select, * home, * start;

	GameObject* player;

	InputDemo() : Game(400, 400, "Input Demo") {
		demo = addScene("demo");
		lAxis = demo->add()
			->addComponentAnd(new CircleRenderer(25))
			->setPosition(Vector(150, 300));

		rAxis = demo->add()
			->addComponentAnd(new CircleRenderer(25))
			->setPosition(Vector(250, 300));

		demo->add()
			->addComponentAnd(new BoxRenderer(350, 150))
			->setPosition(Vector(25, 200));

		a = demo->add()
			->addComponentAnd(new BoxRenderer(25, 25))
			->setPosition(Vector(300, 275));

		b = demo->add()
			->addComponentAnd(new BoxRenderer(25, 25))
			->setPosition(Vector(325, 250));

		x = demo->add()
			->addComponentAnd(new BoxRenderer(25, 25))
			->setPosition(Vector(275, 250));
		y = demo->add()
			->addComponentAnd(new BoxRenderer(25, 25))
			->setPosition(Vector(300, 225));

		lT = demo->add()
			->addComponentAnd(new BoxRenderer(40, 20))
			->setPosition(Vector(50, 180));

		player = demo->add()
			->addComponentAnd(new CircleRenderer(30))
			->setPosition(Vector(200, 50));
	}

	void run() {

		demo->initialize();

		auto gamepad = Gamepad(0);

		while (running) {
			gamepad.pollDevice();
			lAxis->setPosition(Vector(100, 300) + gamepad.lAxis * 25);
			rAxis->setPosition(Vector(300, 300) + gamepad.rAxis * 25);

			
			if (gamepad.a) a->getComponent<BoxRenderer>()->fill = true;
			else a->getComponent<BoxRenderer>()->fill = false;

			if (gamepad.b) b->getComponent<BoxRenderer>()->fill = true;
			else b->getComponent<BoxRenderer>()->fill = false;

			if (gamepad.x) x->getComponent<BoxRenderer>()->fill = true;
			else x->getComponent<BoxRenderer>()->fill = false;

			if (gamepad.y) y->getComponent<BoxRenderer>()->fill = true;
			else y->getComponent<BoxRenderer>()->fill = false;

			if (gamepad.rTrigger) lT->getComponent<BoxRenderer>()->fill = true;
			else lT->getComponent<BoxRenderer>()->fill = false;

			player->move(gamepad.lAxis * (50 * demo->dt_s()));

			tick();
		}
	}
};