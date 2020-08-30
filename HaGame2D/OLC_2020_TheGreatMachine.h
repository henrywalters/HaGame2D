#pragma once
#include "HaGame2D.h"
#include "TGMPlayerController.h"
#include "Polygon.h"
#include "CollisionSystem.h"

namespace TGM {

	const int GAME_WIDTH = 800;
	const int GAME_HEIGHT = 800;

	const int UI_PADDING = 20;
	const int UI_HEIGHT = 100;

	const Color BG_COLOR = Color(0x4F, 0x4F, 0x4F, 0xFF);
	const Color FG_COLOR = Color(0xD1, 0xD1, 0xD1, 0xFF);

	class OLC_2020_TheGreatMachine : public Game
	{
	private:

		Scene* game;
		Scene* ui;

		GameObject* player;

		TGM::PlayerController* playerController;
		CollisionSystem* collisionSystem;

	public:
		OLC_2020_TheGreatMachine() : Game(GAME_WIDTH, GAME_HEIGHT, "The Great Machine") {

			// Initialize Scenes
			game = addScene("game");

			// Set game level variables
			game->display->clearColor = BG_COLOR.rgb;

			// Initialize the player
			player = game->add()
				->addComponentAnd(new SpriteRenderer("../Assets/Sprites/HaGameEngine/Characters/Kitty.png", 50, 50, NULL))
				->addComponentAnd(new TGM::BodyComponent(100))
				->addComponentAnd<CircleCollider>(new CircleCollider(25, false))
				->addComponentAnd<CircleRenderer>(new CircleRenderer(25, false));
			player->origin = Vector(25, 25);
			player->setPosition(Vector(GAME_WIDTH, GAME_HEIGHT) * 0.5);
			player->getComponent<CollisionComponent>()->pollCollisions = true;

			// Initialize global systems
			collisionSystem = new CollisionSystem();

			// Initialize player systems
			playerController = new TGM::PlayerController(player, collisionSystem);  

			// Add systems to game
			game->addSystem(collisionSystem);
			game->addSystem(playerController);
			
			// MISC
			float wallWidth = GAME_WIDTH;
			float wallHeight = 20;

			auto wall1 = game->add()
				->addComponentAnd(new BoxRenderer(wallWidth, wallHeight, true, FG_COLOR.rgb))
				->addComponentAnd(new BoxCollider(wallWidth, wallHeight))
				->setPosition(Vector(0, GAME_HEIGHT / 4));

			auto wall2 = game->add()
				->addComponentAnd(new BoxRenderer(wallWidth, wallHeight, true, FG_COLOR.rgb))
				->addComponentAnd(new BoxCollider(wallWidth, wallHeight))
				->setPosition(Vector(0, 3 * GAME_HEIGHT / 4));

			auto wall3 = game->add()
				->addComponentAnd(new BoxRenderer(wallHeight, wallWidth, true, FG_COLOR.rgb))
				->addComponentAnd(new BoxCollider(wallHeight, wallWidth))
				->setPosition(Vector(GAME_WIDTH / 4, 0));

			auto wall4 = game->add()
				->addComponentAnd(new BoxRenderer(wallHeight, wallWidth, true, FG_COLOR.rgb))
				->addComponentAnd(new BoxCollider(wallHeight, wallWidth))
				->setPosition(Vector(3 * GAME_WIDTH / 4, 0));
		};

		void run() {
			game->initialize();

			while (running) {
				
				tick();
			}
		}
	};
}

