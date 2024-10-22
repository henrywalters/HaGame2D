#include "stdafx.h"
#include "stdafx.h"
#include "Scene.h"
#include "DataSample.h"
#include "Camera.h"
#include "Random.h"

const char * TEMP_GAMEOBJECT_ID = "tmp";

std::unordered_map<char *, GameObject *> Scene::gameObjectTable;

Scene::Scene()
{
	display = NULL;
	input = NULL;
}


Scene::~Scene()
{
	free(display);
	free(quadTree);
	gameObjects.clear();
	gameObjectTable.clear();
}

void Scene::initializeScreen(int sWidth, int sHeight, Display * _display, Input * _input) {

	screenWidth = sWidth;
	screenHeight = sHeight;

	//display = _display; input = _input;

	display = new Display();

	display->window = _display->window;
	display->screen = _display->screen;
	display->renderer = _display->renderer;

	input = _input;

	camera.setScreenDimensions(screenWidth, screenHeight);
	camera.setDisplay(display);

	quadTree = new QuadTree(screenWidth, screenHeight);
	camera.setInput(input);
	quadTree->setDisplay(display);

	screen = Matrix(Vector::Zero(), Vector(screenWidth, screenHeight));
	viewport = Matrix(Vector::Zero(), Vector(screenWidth, screenHeight));
}

void Scene::setDisplayPort(double x, double y, double width, double height) {
	display->setDisplayPort(x, y, width, height);
	viewport = Matrix(Vector(x, y), Vector(width, height));
}

void Scene::initialize() {

	for (auto system : systems) {
		system->setScene(this);
		system->onInit();
	}

	std::vector<GameObject *> objects = getGameObjects();
	int unmappedObjects = 0;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->id != NULL) {
			if (gameObjectTable.find(objects[i]->id) == gameObjectTable.end()) {
				Scene::instantiate(objects[i]->id, objects[i]);
			}
		}
		else {
			unmappedObjects += 1;
			std::stringstream stream;
			stream << unmappedObjects;
			std::string id = stream.str();
			Scene::instantiate((char *) std::to_string(Random::number(0, 100000)).c_str(), objects[i]);
		}

		for (int j = 0; j < objects[i]->componentCount; j++) {
			objects[i]->components[j]->initialize(display, input, this);
			if (hasLogger) {
				objects[i]->components[j]->setLogger(logger);
			}
			objects[i]->components[j]->onCreate();
		}
	}
	gameObjects = objects;
	gameObjectsInitialized = true;
}

void Scene::reset() {
	cleanTree();
	gameObjects.clear();
	gameObjectTable.clear();
	gameObjectsInitialized = false;
}

GameObject * Scene::instantiate(GameObject * gameObject) {
	Scene::instantiate((char *)std::to_string(Random::number(0, 100000)).c_str(), gameObject);
	// gameObject->initialize(display, input, this);
	for (int i = 0; i < gameObject->componentCount; i++) {
		gameObject->components[i]->initialize(display, input, this);
		gameObject->components[i]->onCreate();
	}
	for (auto child : gameObject->childGameObjects) {
		child->parentGameObject = gameObject;
		instantiate(child);
	}
	gameObjects.push_back(gameObject);
	add(gameObject);

	return gameObject;
}

void Scene::destroy(GameObject * gameObject) {
	for (int i = 0; i < gameObject->componentCount; i++) {
		gameObject->components[i]->active = false;
		gameObject->components[i]->onDestroy();
	}

	GameObject* parent = gameObject->parentGameObject;

	if (parent != NULL) {
		std::remove(parent->childGameObjects.begin(), parent->childGameObjects.end(), gameObject);
	}
	else {
		std::cout << "WARNING - not destroying anything because gameObject does not have parent object\n";
	}
	//This MUST be changed soon. 
	gameObject->active = false;

}

void Scene::tick() {

	int startFrame = SDL_GetTicks();

	camera.update();

	Matrix displayPort = display->displayPort;

	input->globalizeMouse(displayPort.get(0), displayPort.get(1), screen, viewport);

	//Iterate through the game objects .. lets do this

	std::queue<GameObject *> objectQueue;
	std::vector<GameObject *> objectBuffer;

	//int quadInsert = SDL_GetTicks();

	/*
	//quadTree = new QuadTree(screenWidth, screenHeight);
	quadTree = new QuadTree(10000, 10000);
	quadTree->setDisplay(display);

	for (int i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->getComponent<BoxCollider>() != NULL) {
			quadTree->insert(gameObjects[i]);
		}
	}
	*/

	for (auto system : systems) {
		system->update();
	}
	

	int tick = SDL_GetTicks();

	objectQueue.push(root);

	while (!objectQueue.empty()) {

		GameObject * currentObject = objectQueue.front();

		objectBuffer.push_back(currentObject);

		//update current object

		if (currentObject->parentGameObject != NULL) {
			if (currentObject->parentGameObject->positionUpdated) {
				currentObject->move(currentObject->parentGameObject->positionDelta);
			}
		}


		int updateStart = SDL_GetTicks();

		currentObject->update();

		objectQueue.pop();

		for (int i = 0; i < currentObject->childGameObjectCount; i++) {
			if (currentObject->childGameObjects[i]->active && (currentObject->trackedOffScreen || currentObject->isWithinViewport(viewport))) {
				objectQueue.push(currentObject->childGameObjects[i]);
			}
		}
	}

	viewport = camera.getViewport();

	for (int i = 0; i < gameObjects.size(); i++) {
		Matrix obj;
		obj.set(0, gameObjects[i]->position.x);
		obj.set(1, gameObjects[i]->position.y);
		BoxComponent * box = gameObjects[i]->getComponent<BoxComponent>();

		if (box != NULL) {
			obj.set(2, box->width);
			obj.set(3, box->height);
		}
		else {
			obj.set(2, 1);
			obj.set(3, 1);
		}

		Matrix screenObj = WorldMapper::worldToScreen(obj, viewport, screen);

		gameObjects[i]->relativePosition = Vector(screenObj.get(0), screenObj.get(1));

		if (box != NULL) {
			box->relativeWidth = screenObj.get(2);
			box->relativeHeight = screenObj.get(3);
		}
	}
	
	//quadTree->clear();

	//fpsData.add(SDL_GetTicks() - startFrame);

}

void Scene::loop() {

	if (!gameObjectsInitialized) {
		std::cout << "WARNING - Game Objects have not been initialized. This means all components have not been given access to primative functionallity. Please call initializeGameObjects() before looping\n\n";
	}

	while (!quit) {
		tick();
	}
}

GameObject * Scene::instantiate(char * key, GameObject * gameObject) {
	gameObjectTable[key] = gameObject;
	gameObject->id = key;
	return gameObject;
}

GameObject * Scene::getGameObject(char * key) {
	GameObject * gameObject = gameObjectTable[key];
	if (gameObject == NULL) {
		//std::cout << "WARNING - Game Object ID: " << key << " does not exist\n";
	}

	return gameObject;
}