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
#include "RacingGame.h"
#include "Platformer.h"

int main(int argc, char* argv[])
{
	// ZeldaClone zelda = ZeldaClone();
	Platformer platformer = Platformer();

	//RacingGame racing = RacingGame();

    //SpriteSheetHelper helper = SpriteSheetHelper(264, 264);
	//SpriteSheetHelper::autoLoad(320, 320, 10, 10, "terrain-tiles.ssd");
	//MapBuilder builder("training-area.map");

	return 0;
}
