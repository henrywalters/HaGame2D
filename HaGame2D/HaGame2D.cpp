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

int main(int argc, char* argv[])
{
	//ZeldaClone zelda = ZeldaClone();

    //SpriteSheetHelper helper = SpriteSheetHelper("../Assets/Sprites/HaGameEngine/Characters/Link.png", 418, 173);

	std::string rows[] = {
		"Clubs",
		"Spades",
		"Hearts",
		"Diamonds"
	};

	std::string cols[] = {
		"Ace",
		"Two",
		"Three",
		"Four",
		"Five",
		"Six",
		"Seven",
		"Eight",
		"Nine",
		"Ten",
		"Jack",
		"Queen",
		"King",
		"REPLACE_ME"
	};

	//SpriteSheetHelper::autoLoad(1008, 384, 4, 14, "../Assets/SSDefinitions/cards.ssd", rows, cols);
	//MapBuilder builder("../Assets/Maps/platformer-level1.map");

	//BlackJack yo;

	CasinoAdventure adventure = CasinoAdventure(1);

	adventure.loop();

	return 0;
}
