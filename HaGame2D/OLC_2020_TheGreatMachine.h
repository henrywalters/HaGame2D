#pragma once
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
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
		"Assets/Levels/level3.txt",
		"Assets/Levels/level2.txt",
		"Assets/Levels/level3.txt",
	};

	const std::string SPLASH_PATH = "Assets/Sprites/TGMHome.png";
	
	const std::string DOOR_PATH = "Assets/Sprites/door.png";
	const std::string END_PATH = "Assets/Sprites/end.png";
	const std::string ENEMY_PATH = "Assets/Sprites/enemy.png";
	const std::string HEALTH_PATH = "Assets/Sprites/health.png";
	const std::string PLAYER_PATH = "Assets/Sprites/player.png";
	const std::string WALL_PATH = "Assets/Sprites/wall.png";

	const std::vector<std::string> levelNames = {
		"The Awakening",
		"Blood Bath",
		"Aftermath",
	};

	const int GAME_WIDTH = 800;
	const int GAME_HEIGHT = 800;

	const int UI_PADDING = 20;
	const int UI_HEIGHT = 100;

	const int BLOCK_WIDTH = 80;
	const int BLOCK_HEIGHT = 80;
	const Vector BLOCK_SIZE = Vector(BLOCK_WIDTH, BLOCK_HEIGHT);
	const Vector HEALTHBOX_SIZE = Vector(BLOCK_WIDTH / 3, BLOCK_HEIGHT / 3);
	const Vector ENEMY_SIZE = Vector(BLOCK_WIDTH * 0.75f, BLOCK_HEIGHT * 0.75f);
	const Vector BTN_SIZE = Vector(800, 50);

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

	const float BTN_MARGIN = 50;

	enum GAME_STATES {
		LOADING,
		MAIN_MENU,
		PLAYING,
		IN_GAME_MENU,
		WIN_SCREEN,
		DEAD_SCREEN,
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

	const std::vector<std::string> mainMenuOptions = {
		"New Game",
		//"Levels",
		"Quit to Desktop",
	};

	const std::vector<std::string> inGameMenuOptions = {
		"Reset Level",
		"Quit",
	};

	const std::vector<std::string> deadMenuOptions = {
		"Reset Level",
		"Quit",
	};

	class OLC_2020_TheGreatMachine : public Game
	{
	private:

		Scene* global;
		Scene* home;
		Scene* gameMenu;
		Scene* ui;
		Scene* winScreen;
		Scene* deadScreen;

		std::vector<Scene*> levelScenes = {};

		GAME_STATES state;

		int elapsedSeconds;

		int currentLevelNum = -1;

		int mainMenuOption = 0;
		int inGameMenuOption = 0;
		int deadScreenMenuOption = 0;

		std::vector<GameObject*> mainMenuBtns;
		std::vector<GameObject*> inGameMenuBtns;
		std::vector<GameObject*> deadMenuBtns;

	public:
		OLC_2020_TheGreatMachine() : Game(GAME_WIDTH, GAME_HEIGHT, "GOOM - OLC 2020: The Great Machine") {

			state = GAME_STATES::LOADING;

			deadScreen = addScene("dead", false);
			gameMenu = addScene("gameMenu", false);
			global = addScene("global", true);
			home = addScene("home", false);
			winScreen = addScene("win", false);
			

			// Home screen
			home->add()
				->addComponentAnd(new SpriteRenderer(SPLASH_PATH, GAME_WIDTH, GAME_HEIGHT, NULL));

			addLabel(home, Vector(0, GAME_HEIGHT / 2.0f - 100), Vector(GAME_WIDTH, 75), 48, WHITE, "GOOM");
			mainMenuBtns = addMenuButtons(home, mainMenuOptions, Vector(0, GAME_HEIGHT / 2.0f - 50.0), BTN_SIZE, 36, WHITE);

			// Win screen
			addLabel(winScreen, Vector(0, GAME_HEIGHT / 2.0f - 200), Vector(GAME_WIDTH, 75), 48, WHITE, "You've Won!");
			addLabel(winScreen, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f ), Vector(BTN_SIZE.x, BTN_SIZE.y), 36, WHITE, "Thanks for playing");
			addLabel(winScreen, Vector(GAME_WIDTH / 2 - BTN_SIZE.x / 2, GAME_HEIGHT / 2.0f + 150.0f), Vector(BTN_SIZE.x, BTN_SIZE.y), 28, WHITE, "Developed by Henry Walters");

			// Dead Screen
			addLabel(deadScreen, Vector(0, GAME_HEIGHT / 2.0f - 100), Vector(GAME_WIDTH, 75), 48, WHITE, "You've Died!");
			deadMenuBtns = addMenuButtons(deadScreen, deadMenuOptions, Vector(0, GAME_HEIGHT / 2.0f - 50.0), BTN_SIZE, 36, WHITE);

		
			// In Game Menu
			addLabel(gameMenu, Vector(0, GAME_HEIGHT / 2.0f - 100), Vector(GAME_WIDTH, 75), 48, WHITE, "Paused");
			inGameMenuBtns = addMenuButtons(gameMenu, inGameMenuOptions, Vector(0, GAME_HEIGHT / 2.0f - 50.0), BTN_SIZE, 36, WHITE);

			// MISC
			float wallWidth = GAME_WIDTH;
			float wallHeight = 20;

			createLevels();

			changeState(GAME_STATES::MAIN_MENU);
		};

		void run() {

			setFps(60);

			global->initialize();
			home->initialize();
			gameMenu->initialize();
			winScreen->initialize();
			deadScreen->initialize();

			while (running) {
				tick();
				if (global->input->esc) {
					if (state == GAME_STATES::PLAYING) {
						changeState(GAME_STATES::IN_GAME_MENU);
					}
					else if (state == GAME_STATES::IN_GAME_MENU) {
						changeState(GAME_STATES::PLAYING);
					}
				}

				if (state == GAME_STATES::MAIN_MENU) {
					handleMenuItem(mainMenuOption, mainMenuBtns);
					if (home->input->enterDown) {
						if (mainMenuOption == 0) {
							nextLevel();
						}
						else if (mainMenuOption == 1) {
							quitToDesktop();
						}
					}
				}

				if (state == GAME_STATES::IN_GAME_MENU) {
					handleMenuItem(inGameMenuOption, inGameMenuBtns);
					if (home->input->enterDown) {
						if (inGameMenuOption == 0) {
							resetLevel(currentLevelNum);
						}
						else if (inGameMenuOption == 1) {
							quit();
						}
					}
				}

				if (state == GAME_STATES::DEAD_SCREEN) {
					handleMenuItem(deadScreenMenuOption, deadMenuBtns);
					if (home->input->enterDown) {
						if (deadScreenMenuOption == 0) {
							resetLevel(currentLevelNum);
						}
						else if (deadScreenMenuOption == 1) {
							quit();
						}
					}
				}
			}
		}

		void handleMenuItem(int& item, std::vector<GameObject*> items) {
			int size = items.size();
			if (global->input->upPressed) {
				item = Math::clamp(item - 1, 0, size - 1);
			}
			if (global->input->downPressed) {
				item = Math::clamp(item + 1, 0, size - 1);
			}

			for (int i = 0; i < size; i++) {
				if (i == item) items[i]->getComponent<TextRenderer>()->setFontColor(RED.rgb);
				else items[i]->getComponent<TextRenderer>()->setFontColor(WHITE.rgb);
			}
		}

		void changeState(GAME_STATES _state) {
			state = _state;

			deactivateScene("gameMenu");
			deactivateScene("home");
			deactivateScene("win");
			deactivateScene("dead");

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
			if (state == GAME_STATES::DEAD_SCREEN) activateScene("dead");
		}

		void quit() {
			mainMenuOption = 0;
			inGameMenuOption = 0;
			deadScreenMenuOption = 0;

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
			if (currentLevelNum == levels.size() - 1) {
				changeState(GAME_STATES::WIN_SCREEN);
			}
			else {
				currentLevelNum++;
				changeState(GAME_STATES::PLAYING);
			}
		}

		Scene* currentLevel() {
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

				playerController->onDead = [this]() {
					if (state != GAME_STATES::DEAD_SCREEN) {
						changeState(GAME_STATES::DEAD_SCREEN);
					}
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
			std::cout << "Clearing Level: " << levelIndex << std::endl;
			levelScenes[levelIndex]->reset();
			std::cout << "RESET LEVEL\n";
			loadLevel(levelScenes[levelIndex], LevelLoader::load(levels[levelIndex]));
			std::cout << "LOADED LEVEL\n";
			levelScenes[levelIndex]->initialize();
			std::cout << "Initialized Level\n";
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

		std::vector<GameObject*> addMenuButtons(Scene* scene, std::vector<std::string> buttonNames, Vector startPos, Vector btnSize, float fontSize, Color color) {
			auto btns = std::vector<GameObject*>();
			int index = 0;
			for (auto name : buttonNames) {
				btns.push_back(
					addLabel(scene, startPos + Vector(0, index * btnSize.y + BTN_MARGIN), btnSize, fontSize, color, name)
				);
				index++;
			}
			return btns;
		}

		GameObject* addPlayer(Scene *scene) {
			auto player = scene->add()
				->addComponentAnd(new SpriteRenderer(PLAYER_PATH, 50, 50, NULL))
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
				->addComponentAnd(new SpriteRenderer(END_PATH, rect.width, rect.height, NULL))
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
				->addComponentAnd(new SpriteRenderer(ENEMY_PATH, rect.width, rect.height, NULL))
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
				->addComponentAnd(new SpriteRenderer(HEALTH_PATH, rect.width, rect.height, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->addComponentAnd(new HealthPickup(value))
				->addTagAnd("HEALTH");
		}

		GameObject* addDoor(Scene* scene, Box rect, Color color, bool open) {
			return scene->add()
				->addComponentAnd(new TGM::Door(Vector(rect.width, rect.height), color, open))
				->addComponentAnd(new SpriteRenderer(DOOR_PATH, BLOCK_SIZE.x, BLOCK_SIZE.y, NULL))
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
			wall->addComponentAnd(new SpriteRenderer(WALL_PATH, BLOCK_SIZE.x, BLOCK_SIZE.y, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->setPosition(Vector(rect.x, rect.y));
			return wall;
		}
	};
}

