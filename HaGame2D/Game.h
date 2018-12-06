#pragma once
#include "Display.h"
#include "Input.h"
#include "Scene.h"
#include <string>
#include <unordered_map>

const float DEFAULT_ASPECT_RATIO = 4 / 3;
const int DEFAULT_HEIGHT = 600;
const int DEFAULT_WIDTH = DEFAULT_HEIGHT * DEFAULT_ASPECT_RATIO;
const int FPS = 60;
//const char * DEFAULT_WINDOW_TITLE = "HaGame2D: Untitled Game";

struct GameScene {
	Scene * scene;
	bool active;
};

class Game
{
	char * title;
	std::unordered_map<std::string, GameScene> scenes;
	std::vector<std::string> keys;
	std::vector<std::string> activeKeys;
	Display * display;
	Input * input;

public:
	Game(int _screenWidth = DEFAULT_WIDTH, int _screenHeight = DEFAULT_HEIGHT, char * title = "HaGame2D: Untitled Game");
	~Game();

	bool running = true;

	int screenWidth, screenHeight;

	Scene * addScene(std::string tag, bool active = true);
	Scene * getScene(std::string tag);
	bool sceneExists(std::string tag);

	void activateScene(std::string tag);
	void deactivateScene(std::string tag);

	//before loading the game, goes through all of the scenes and filters out only those which are active.

	void prepareScene();

	void tick();
};

