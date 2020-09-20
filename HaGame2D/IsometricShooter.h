#pragma once
#include "HaGame2D.h"
#include "PolygonRenderer.h"

const float ANGLE = -(45.0 * M_PI) / 180;

using Quadrilateral = Polygon<4>;

class IsometricShooter : public Game {
public:

	Scene* game;

	GameObject* player;

	IsometricShooter() : Game(1200, 600, "ISOShooter") {
		game = addScene("game");

		addFloorTile(Box{ 300, 300, 2000, 1000 });

		player = game->add()
			->addComponentAnd(new BoxRenderer(50, 50))
			->setPosition(Vector(100, 275));
	}

	void run() {

		auto gamepad = Gamepad(0);

		game->initialize();

		while (running) {

			gamepad.pollDevice();

			player->move(gamepad.lAxis * 100 * game->dt_s());

			if (game->input->fire1Down) {
				addFloorTile(Box{(double) game->input->mouseXGlobal, (double) game->input->mouseYGlobal, 100, 100 });

			}

			game->camera.position(player->position - Vector(575, 275));

			tick();
		}
	}

	GameObject* addFloorTile(Box rect) {
		auto poly = mapRectToPolygon(Box{ 0, 0, rect.width, rect.height });
		auto tile = game->add()
			->addComponentAnd(new PolygonRenderer<4>(poly))
			->setPosition(rect.position());
		game->instantiate(tile);
		return tile;
	}

	Quadrilateral mapRectToPolygon(Box rect) {
		Quadrilateral poly;
		poly.vertices[0] = Vector(rect.x, rect.y);
		poly.vertices[1] = Vector(rect.x + rect.width, rect.y);
		poly.vertices[2] = Vector(rect.x + rect.width + cos(ANGLE) * rect.height, rect.y + sin(ANGLE) * rect.height);
		poly.vertices[3] = Vector(rect.x + cos(ANGLE) * rect.height, rect.y + sin(ANGLE) * rect.height);
		return poly;
	}
};