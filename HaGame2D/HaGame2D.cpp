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

int main(int argc, char* argv[])
{
	ZeldaClone zelda = ZeldaClone();

    //SpriteSheetHelper helper = SpriteSheetHelper(320, 320);
	//SpriteSheetHelper::autoLoad(320, 320, 10, 10, "terrain-tiles.ssd");
	//MapBuilder builder("training-area.map");

	return 0;
}
