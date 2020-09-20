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
#include "Util.h"
#include "System.h"

class System;

class Scene : public GameObjectTree
{
protected:
	bool quit = false;

	bool gameObjectsInitialized = false;

	QuadTree *quadTree;

	std::vector<GameObject *> gameObjects;

	std::vector<System*> systems;

	std::vector<double> framerateSamples;

	const double SAMPLE_SIZE = 60;

	static std::unordered_map<char *, GameObject *> gameObjectTable;

	bool hasLogger = false;

	double _dt_ms;

public:

	Display * display;
	Input * input;

	Logger *logger;

	//Profiling objects
	DataSample<double> fpsData;
	DataSample<double> drawTime;
	DataSample<double> updateTime;
	Camera camera;

	int screenWidth;
	int screenHeight;

	Matrix viewport;
	Matrix screen;

	int FPS = 60;

	Vector origin = Vector::Zero();

	Scene();
	~Scene();

	void initializeScreen(int sWidth, int sHeight, Display * _display, Input * _input);
	void initialize();

	void reset();

	void setDisplayPort(double x, double y, double width, double height);

	void tick();
	void loop();

	void setLogger(Logger *_logger) {
		logger = _logger;
		hasLogger = true;
	}

	static GameObject * instantiate(char * id, GameObject * gameObject);
	GameObject * instantiate(GameObject * gameObject);
	
	void destroy(GameObject * gameObject);

	static GameObject* getGameObject(char* id);

	System* addSystem(System* system) {
		systems.push_back(system);
		return system;
	}

	Vector screenCenter() {
		return Vector(screenWidth / 2, screenHeight / 2);
	}

	void setDt(double dt_ms) {
		_dt_ms = dt_ms;
	}

	double dt() {
		return _dt_ms;
	}

	double dt_s() {
		return _dt_ms / 1000000;
	}

	Vector screenSize() {
		return Vector(screenWidth, screenHeight);
	}
};

