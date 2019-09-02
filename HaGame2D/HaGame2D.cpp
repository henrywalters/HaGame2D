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

int main(int argc, char* argv[])
{

	Platformer platformer = Platformer();
	//ZeldaClone zelda = ZeldaClone();

    // SpriteSheetHelper helper = SpriteSheetHelper("../Assets/Sprites/HaGameEngine/Characters/mario-3.gif", 292, 177);

	// CasinoAdventure adventure = CasinoAdventure(1);

	// adventure.loop();

	return 0;
}
