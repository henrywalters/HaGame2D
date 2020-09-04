#pragma once
#include "HaGame2D.h"
#include "TGMPlayerController.h"
#include "TGMEnemies.h"
#include "Polygon.h"
#include "CollisionSystem.h"
#include "TGMTriggerReceiver.h"
#include "TGMLevers.h"
#include "TGMPlatform.h"
#include "TGMDoor.h"
#include "TGMLevelLoader.h"
#include "TGMHealthBar.h"

namespace TGM {

	const std::vector<std::string> levels = {
		"../Assets/TGM/Levels/level1.txt",
		"../Assets/TGM/Levels/level2.txt"
	};

	const std::vector<std::string> levelNames = {
		"The Awakening",
		"Blood Bath"
	};

	const int GAME_WIDTH = 800;
	const int GAME_HEIGHT = 800;

	const int UI_PADDING = 20;
	const int UI_HEIGHT = 100;

	const int BLOCK_WIDTH = 120;
	const int BLOCK_HEIGHT = 120;
	const Vector BLOCK_SIZE = Vector(BLOCK_WIDTH, BLOCK_HEIGHT);
	const Vector HEALTHBOX_SIZE = Vector(BLOCK_WIDTH / 3, BLOCK_HEIGHT / 3);
	const Vector ENEMY_SIZE = Vector(BLOCK_WIDTH * 0.75f, BLOCK_HEIGHT * 0.75f);
	const Vector BTN_SIZE = Vector(300, 50);

	const Color BG_COLOR = Color(0x4F, 0x4F, 0x4F, 0xFF);
	const Color FG_COLOR = Color(0xD1, 0xD1, 0xD1, 0xFF);
	const Color PLASMA_COLOR = Color(0x1B, 0xBF, 0xD1, 0xFF);

	const Color WHITE = Color(0xFF, 0xFF, 0xFF, 0xFF);
	const Color BLACK = Color(0x00, 0x00, 0x00, 0xFF);
	const Color LIGHT_BLUE = Color(42, 210, 247, 255);
	const Color BLUE = Color(0x00, 0x00, 0xFF, 0xFF);
	const Color GREEN = Color(0x00, 0xFF, 0x00, 0xFF);
	const Color RED = Color(0xFF, 0x00, 0x00, 0xFF);
	const Color PURPLE = Color(162, 9, 232, 255);
	const Color ORANGE = Color(232, 132, 9, 255);

	enum GAME_STATES {
		LOADING,
		MAIN_MENU,
		PLAYING,
		IN_GAME_MENU,
		WIN_SCREEN,
	};

	enum BLOCK_TYPES {
		WALL_BLOCK,
		LEVER_BLOCK,
		DOOR_BLOCK,
		START_BLOCK,
		FINISH_BLOCK,
		HEALTH_BLOCK,
		ENEMY_BLOCK
	};

	const std::vector<Color> BLOCK_COLORS = {
		FG_COLOR,
		WHITE,
		BLUE,
		GREEN,
	};

	class OLC_2020_TheGreatMachine : public Game
	{
	private:

		Scene* global;
		Scene* home;
		// Scene* game;
		Scene* gameMenu;
		Scene* ui;
		Scene* winScreen;

		std::vector<Scene*> levelScenes = {};

		GAME_STATES state;

		//TGM::PlayerController* playerController;
		//TGM::EnemySystem* enemySystem;
		//TGM::TriggerReceiverSystem* trSystem;
		//CollisionSystem* collisionSystem;

		int elapsedSeconds;

		int currentLevelNum = -1;

	public:
		OLC_2020_TheGreatMachine() : Game(GAME_WIDTH, GAME_HEIGHT, "The Great Machine") {

			state = GAME_STATES::LOADING;

			// Initialize Scenes
			// game = addScene("game", false);
			gameMenu = addScene("gameMenu", false);
			global = addScene("global", true);
			home = addScene("home", false);
			winScreen = addScene("win", false);

			// Home screen
			home->add()
				->addComponentAnd(new SpriteRenderer("../Assets/Sprites/HaGameEngine/TGMHome.png", GAME_WIDTH, GAME_HEIGHT, NULL));

			
			addLabel(home, Vector(0, GAME_HEIGHT / 2.0f - 37.5), Vector(GAME_WIDTH, 75), 48, WHITE, "The Great Machine");
			addLabel(home, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f + 20), Vector(BTN_SIZE.x, BTN_SIZE.y), 36, WHITE, "New Game")
				->addComponentAnd(new BoxRenderer(BTN_SIZE.x, BTN_SIZE.y, false))
				->addComponent(new ButtonComponent(BTN_SIZE.x, BTN_SIZE.y))
				->onClickFunc = [this]() {
				nextLevel();
			};

			addLabel(home, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f + 120.0f), Vector(BTN_SIZE.x, BTN_SIZE.y), 36, WHITE, "Quit to Desktop")
				->addComponentAnd(new BoxRenderer(BTN_SIZE.x, BTN_SIZE.y, false))
				->addComponent(new ButtonComponent(BTN_SIZE.x, BTN_SIZE.y))
				->onClickFunc = [this]() {
				quitToDesktop();
			};

			// Win screen
			addLabel(winScreen, Vector(0, GAME_HEIGHT / 2.0f - 200), Vector(GAME_WIDTH, 75), 48, WHITE, "You've Won!");
			addLabel(winScreen, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f ), Vector(BTN_SIZE.x, BTN_SIZE.y), 36, WHITE, "Thanks for playing");

			addLabel(winScreen, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f + 150.0f), Vector(BTN_SIZE.x, BTN_SIZE.y), 28, WHITE, "Developed by Henry Walters");

			// In Game Menu
			addLabel(gameMenu, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f - 50.0f), Vector(BTN_SIZE.x, BTN_SIZE.y), 36, WHITE, "Reset")
				->addComponentAnd(new BoxRenderer(BTN_SIZE.x, BTN_SIZE.y, false))
				->addComponent(new ButtonComponent(BTN_SIZE.x, BTN_SIZE.y))
				->onClickFunc = [this]() {
				resetLevel(currentLevelNum);
			};

			addLabel(gameMenu, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f + 50.0f), Vector(BTN_SIZE.x, BTN_SIZE.y), 36, WHITE, "Quit")
				->addComponentAnd(new BoxRenderer(BTN_SIZE.x, BTN_SIZE.y, false))
				->addComponent(new ButtonComponent(BTN_SIZE.x, BTN_SIZE.y))
				->onClickFunc = [this]() {
				quit();
			};

			// MISC
			float wallWidth = GAME_WIDTH;
			float wallHeight = 20;

			createLevels();

			activateScene("home");
		};

		void run() {
			global->initialize();
			home->initialize();
			gameMenu->initialize();
			winScreen->initialize();

			while (running) {
				if (global->input->esc) {
					if (state == GAME_STATES::PLAYING) {
						changeState(GAME_STATES::IN_GAME_MENU);
					}
					else if (state == GAME_STATES::IN_GAME_MENU) {
						changeState(GAME_STATES::PLAYING);
					}
				}

				tick();
			}
		}

		void changeState(GAME_STATES _state) {
			state = _state;

			deactivateScene("game");
			deactivateScene("gameMenu");
			deactivateScene("home");
			deactivateScene("win");

			std::cout << "LEVELS: " << levels.size();

			for (int i = 0; i < levels.size(); i++) {
				deactivateScene(levelNames[i]);
			}

			if (state == GAME_STATES::MAIN_MENU) {
				home->initialize();
				activateScene("home");
			}
			if (state == GAME_STATES::IN_GAME_MENU) activateScene("gameMenu");
			if (state == GAME_STATES::PLAYING) activateScene(levelNames[currentLevelNum]);
			if (state == GAME_STATES::WIN_SCREEN) activateScene("win");
		}

		void quit() {
			currentLevelNum = -1;
			for (int i = 0; i < levels.size(); i++) {
				clearLevel(i);
			}
			changeState(GAME_STATES::MAIN_MENU);
		}

		void quitToDesktop() {
			running = false;
		}

		void nextLevel() {
			std::cout << currentLevelNum << " vs " << levels.size() << std::endl;
			if (currentLevelNum == levels.size() - 1) {
				changeState(GAME_STATES::WIN_SCREEN);
			}
			else {
				currentLevelNum++;
				changeState(GAME_STATES::PLAYING);
			}
		}

		Scene* currentLevel() {
			std::cout << "Level: " << currentLevelNum << std::endl;
			return levelScenes[currentLevelNum];
		}

		void createLevels() {
			for (int i = 0; i < levels.size(); i++) {
				auto game = addScene(levelNames[i], false);
				// Set game level variables
				game->display->clearColor = BG_COLOR.rgb;

				// Initialize global systems
				auto collisionSystem = new CollisionSystem();
				auto trSystem = new TriggerReceiverSystem();

				// Initialize enemy systems
				auto enemySystem = new TGM::EnemySystem(collisionSystem);
				auto playerController = new TGM::PlayerController(collisionSystem, trSystem);
				playerController->onLevelComplete = [this]() {
					nextLevel();
				};

				// Add systems to game
				game->addSystem(collisionSystem);
				game->addSystem(trSystem);

				game->addSystem(enemySystem);
				game->addSystem(playerController);

				loadLevel(game, LevelLoader::load(levels[i]));

				game->initialize();
				playerController->initialize();

				levelScenes.push_back(game);

				std::cout << "Added " << levelNames[i] << std::endl;
			}
		}

		void clearLevel(int levelIndex) {
			levelScenes[levelIndex]->reset();
			loadLevel(levelScenes[levelIndex], LevelLoader::load(levels[levelIndex]));
			levelScenes[levelIndex]->initialize();
		}

		void resetLevel(int levelIndex) {
			clearLevel(levelIndex);
			changeState(GAME_STATES::PLAYING);
		}

		void loadLevel(Scene* scene, LevelData data) {	

			auto player = addPlayer(scene);

			std::vector<LevelCell*> leverCells = std::vector<LevelCell*>();
			auto doors = std::unordered_map<int, Receiver*>();
			auto receivers = std::unordered_map<int, std::vector<Receiver*>>();

			for (auto cell : data.cells) {

				Box cellBox = Box{ cell->x * BLOCK_SIZE.x, cell->y * BLOCK_SIZE.y, BLOCK_SIZE.x, BLOCK_SIZE.y };

				if (cell->cellType == BLOCK_TYPES::WALL_BLOCK) {
					auto wall = addWall(scene, cellBox, BLOCK_COLORS[cell->cellType]);
				}

				if (cell->cellType == BLOCK_TYPES::DOOR_BLOCK) {
					auto door = addDoor(scene, cellBox, BLOCK_COLORS[cell->cellType], false);
					doors[cell->id] = door->getComponent<Receiver>();
					doors[cell->id]->onReceive([this, door](int val) {
						door->getComponent<Door>()->open = val == 1;
					});
				}


				if (cell->cellType == BLOCK_TYPES::HEALTH_BLOCK) {
					auto healthBox = addHealthBox(scene, Box{
						cell->x * BLOCK_SIZE.x + BLOCK_SIZE.x / 2 - HEALTHBOX_SIZE.x / 2, 
						cell->y * BLOCK_SIZE.y + BLOCK_SIZE.y / 2 - HEALTHBOX_SIZE.y / 2, 
						HEALTHBOX_SIZE.x, 
						HEALTHBOX_SIZE.y 
						}, 25);
				}

				if (cell->cellType == BLOCK_TYPES::LEVER_BLOCK) {
					leverCells.push_back(cell);
				}

				if (cell->cellType == BLOCK_TYPES::START_BLOCK) {
					player->setPosition(Vector(cell->x * BLOCK_SIZE.x, cell->y * BLOCK_SIZE.y));
				}

				if (cell->cellType == BLOCK_TYPES::ENEMY_BLOCK) {
					addEnemy(scene, Box{ cell->x * BLOCK_SIZE.x, cell->y * BLOCK_SIZE.y, BLOCK_SIZE.x, BLOCK_SIZE.y });
				}

				if (cell->cellType == BLOCK_TYPES::FINISH_BLOCK) {
					addEndPoint(scene, Box{ cell->x * BLOCK_SIZE.x, cell->y * BLOCK_SIZE.y, BLOCK_SIZE.x, BLOCK_SIZE.y });
				}
			}

			for (auto connection : data.connections) {
				if (receivers.find(connection->cellA->id) == receivers.end()) {
					receivers[connection->cellA->id] = {};
				}
				receivers[connection->cellA->id].push_back(doors[connection->cellB->id]);
			}

			for (auto lever : leverCells) {
				auto leverObj = addLever(scene, Box{ lever->x * BLOCK_SIZE.x, lever->y * BLOCK_SIZE.y, BLOCK_SIZE.x, BLOCK_SIZE.y }, FG_COLOR, receivers[lever->id]);
			}
		}

		GameObject* addPlayer(Scene *scene) {
			auto player = scene->add()
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/player.png", 50, 50, NULL))
				->addComponentAnd(new TGM::Body(10))
				->addComponentAnd(new TGM::HealthBar())
				->addComponentAnd<CircleCollider>(new CircleCollider(25, false))
				->addTagAnd("PLAYER");

			player->z_index = 15;
			player->origin = Vector(25, 25);
			player->setPosition(Vector(GAME_WIDTH, GAME_HEIGHT) * 0.5);
			player->getComponent<CollisionComponent>()->pollCollisions = true;

			return player;
		}

		GameObject* addEndPoint(Scene* scene, Box rect) {
			return scene->add()
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/end.png", rect.width, rect.height, NULL))
				->setPosition(rect.position())
				->addComponentAnd(new BoxCollider(rect.size()))
				->addTagAnd("END");
		}

		GameObject* addLabel(Scene* scene, Vector pos, Vector size, int fontSize, Color color, std::string msg) {
			auto text = scene->add();
			text->move(pos);
			auto renderer = text->addComponent(new TextRenderer(size.x, size.y));
			renderer->setAllignment(TextAllignments::Center);
			renderer->setFontSize(fontSize);
			renderer->setFontColor(color.rgb);
			renderer->setMessage(msg);
			return text;
		}

		GameObject* addEnemy(Scene* scene, Box rect) {
			auto enemy = scene->add()
				->setPosition(rect.position())
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/enemy.png", rect.width, rect.height, NULL))
				->addComponentAnd(new CircleCollider(rect.width / 2, false))
				->addComponentAnd(new HealthBar())
				->addComponentAnd(new Enemy())
				->addTagAnd("WALL")
				->addTagAnd("ENEMY");

			enemy->origin = rect.size() * 0.5;

			enemy->getComponent<CircleCollider>()->pollCollisions = true;

			return enemy;
		}

		GameObject* addHealthBox(Scene* scene, Box rect, float value) {
			return scene->add()
				->setPosition(rect.position())
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/health.png", rect.width, rect.height, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->addComponentAnd(new HealthPickup(value))
				->addTagAnd("HEALTH");
		}

		GameObject* addDoor(Scene* scene, Box rect, Color color, bool open) {
			return scene->add()
				->addComponentAnd(new TGM::Door(Vector(rect.width, rect.height), color, open))
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/door.png", BLOCK_SIZE.x, BLOCK_SIZE.y, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->addComponentAnd(new Receiver())
				->setPosition(Vector(rect.x, rect.y))
				->addTagAnd("WALL");
		}

		GameObject* addLever(Scene* scene, Box rect, Color color, std::vector<Receiver*> receivers) {
			Vector collPadding = Vector(40, 40);
			auto lever = scene->add()->setPosition(Vector(rect.x, rect.y))
				->addComponentAnd(new BoxRenderer(rect.width, rect.height));

			auto label = (new GameObject())
				->addComponentAnd(new BoxComponent(rect.width, rect.height))
				->addComponentAnd(new LeverDisplay(FG_COLOR, RED, GREEN))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->addTagAnd("WALL");

			auto handler = (new GameObject())
				// ->addComponentAnd(new BoxRenderer(rect.width + collPadding.x, rect.height + collPadding.y, false, Color::green()))
				->addComponentAnd(new BoxCollider(rect.width + collPadding.x, rect.height + collPadding.y))
				->addComponentAnd(new TGM::Trigger(receivers))
				->addTagAnd("TRIGGER")
				->setPosition(collPadding * -0.5f);

			lever->add(label);
			lever->add(handler);

			return lever;
		}

		GameObject* addWall(Scene* scene, Box rect, Color color) {
			GameObject* wall = scene->add();
			wall->tag = "WALL";
			wall->addTag("WALL");
			wall->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/wall.png", BLOCK_SIZE.x, BLOCK_SIZE.y, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->setPosition(Vector(rect.x, rect.y));
			return wall;
		}
	};
}

