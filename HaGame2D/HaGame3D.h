#pragma once
#include "HaGame2D.h"
#include "Display3D.h"

class HaGame3D
{
public:
	HaGame3D() {

		const int width = 600;
		const int height = 600;

		Game game = Game(width, height, "HaGame3D");

		game.prepareScene();

		while (game.running) {
			game.tick();
		}
	}
};

