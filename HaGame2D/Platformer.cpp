#include "stdafx.h"
#include "Platformer.h"

Platformer::Platformer()
{
	Game game = Game(1000, 400, "HaPlatform");


	game.prepareScene();
	while (game.running) {
		game.tick();
	}
}
