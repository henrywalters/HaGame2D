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
#include "PlatformerMapBuilder.h"
#include "OLC_2020_TheGreatMachine.h"
#include "TGMLevelBuilder.h"
#include "Sandbox.h"
#include "InputDemo.h"
#include "IsometricShooter.h"
#include "RaycastTest.h"

int main(int argc, char* argv[])
{
	//MapBuilder("../Assets/maptest");

	//sandbox();
	//auto inputDemo = InputDemo();
	//inputDemo.run();

	//auto shooter = IsometricShooter();
	//shooter.run();

	//auto levelBuilder = TGM::LevelBuilder();
	//levelBuilder.run();

	//auto olc = TGM::OLC_2020_TheGreatMachine();
	//olc.run();

	//auto builder = PlatformerMapBuilder();
	//builder.run();

	auto rcTest = RaycastTest(1600, 1000);
	rcTest.run();

	return 0;
}
