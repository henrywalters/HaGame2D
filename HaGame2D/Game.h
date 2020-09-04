#pragma once
#include "Display.h"
#include "Input.h"
#include "Scene.h"
#include "Component.h"
#include <string>
#include <unordered_map>
#include "Logger.h"
#include "Clock.h"

const float DEFAULT_ASPECT_RATIO = 4 / 3;
const int DEFAULT_HEIGHT = 600;
const int DEFAULT_WIDTH = DEFAULT_HEIGHT * DEFAULT_ASPECT_RATIO;
const int FPS = 60;
const bool CAP_FPS = false;
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
	std::unordered_map<std::string, bool> sceneStates;

	Logger * loggerHandle;

	float averageFps;
	DataSample<float> fpsMeter;

	Clock clock;
	float maxTick;

protected:
	int _fps;
	bool _capFps;

public:
	Display * display;
	Input * input;

	Game(int _screenWidth = DEFAULT_WIDTH, int _screenHeight = DEFAULT_HEIGHT, char * title = "HaGame2D: Untitled Game");
	~Game();

	bool running = true;

	int screenWidth, screenHeight;

	void setFps(int fps, bool fpsCapped = true);

	Scene * addScene(std::string tag, bool active = true);
	Scene * getScene(std::string tag);
	bool sceneExists(std::string tag);

	void activateScene(std::string tag);
	void deactivateScene(std::string tag);
	void resetScene(std::string tag);

	void setLogger(Logger *logger) {
		loggerHandle = logger;
	}

	//before loading the game, goes through all of the scenes and filters out only those which are active.

	void prepareScene();

	void tick();

	std::vector<std::string> getScenes() {
		return keys;
	}

	std::vector<std::string> getActiveScenes() {
		std::vector<std::string> activeKeys;

		for (auto key : keys) {
			if (sceneStates[key]) {
				activeKeys.push_back(key);
			}
		}

		return activeKeys;
	}
};

