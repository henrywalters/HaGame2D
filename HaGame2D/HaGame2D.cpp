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

GameObject* newRect(Scene *scene, Vector pos, Vector size) {
	return scene->add()
		->setPosition(pos)
		->addComponentAnd(new BoxRenderer(size.x, size.y))
		->addComponentAnd(new BoxCollider(size.x, size.y));
}

int main(int argc, char* argv[])
{
	//MapBuilder("../Assets/maptest");

	//sandbox();

	auto levelBuilder = TGM::LevelBuilder();
	levelBuilder.run();

	// auto olc = OLC_2020_TheGreatMachine();
	// olc.run();

	//auto builder = PlatformerMapBuilder();
	//builder.run();

	return 0;
}
