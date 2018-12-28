#pragma once

#include "Display.h"
#include "Input.h"
#include <vector>
#include "Matrix.h"
#include <unordered_map>
#include "GameObjectTree.h"
#include "GameObject.h"
#include "BoxRenderer.h"
#include "CollisionComponent.h"
#include "BoxCollider.h"
#include "OrthogonolCharacterController.h"
#include "WorldMapper.h"
#include "QuadTree.h"
#include <sstream>
#include <string>
#include <queue>
#include "Camera.h"
#include "DataSample.h"
#include "Logger.h"

class Scene : public GameObjectTree
{
protected:
	bool quit = false;

	bool gameObjectsInitialized = false;

	QuadTree *quadTree;


	std::vector<GameObject *> gameObjects;

	std::vector<float> framerateSamples;

	const float SAMPLE_SIZE = 60;

	static std::unordered_map<char *, GameObject *> gameObjectTable;

	bool hasLogger = false;
	Logger logger;

public:

	Display * display;
	Input * input;

	//Profiling objects
	DataSample<float> fpsData;
	DataSample<float> drawTime;
	DataSample<float> updateTime;
	Camera camera;

	int screenWidth;
	int screenHeight;

	Matrix viewport;
	Matrix screen;

	int FPS = 60;

	Scene();
	~Scene();

	void initialize(int sWidth, int sHeight, Display * _display, Input * _input);
	void initializeGameObjects();

	void setDisplayPort(float x, float y, float width, float height);

	void tick();
	void loop();

	void setLogger(Logger _logger) {
		logger = _logger;
		hasLogger = true;

		std::cout << "Set Logger!\n";
	}


	void log(std::string message) {
		if (hasLogger) {
			logger.log(message);
		}
		else {
			std::cout << "WARNING - can not log message because no logger has been set\n";
		}
	}

	static GameObject * instantiate(char * id, GameObject * gameObject);
	GameObject * instantiate(GameObject * gameObject);
	
	void destroy(GameObject * gameObject);

	static GameObject * getGameObject(char * id);

	Vector screenCenter() {
		return Vector(screenWidth / 2, screenHeight / 2);
	}
};

