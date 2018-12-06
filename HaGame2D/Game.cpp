#include "stdafx.h"
#include "Game.h"


Game::Game(int _screenWidth, int _screenHeight, char * _title)
{
	title = _title;
	screenWidth = _screenWidth; screenHeight = _screenHeight;
	display = new Display(screenWidth, screenHeight, title);
	display->setFont(24, DOROVAR_FONT);
	input = new Input();
}


Game::~Game()
{
}

Scene * Game::addScene(std::string tag, bool active) {
	GameScene scene;
	scene.active = active;
	scene.scene = new Scene();
	scene.scene->initialize(screenWidth, screenHeight, display, input);

	if (!sceneExists(tag)) {
		scenes[tag] = scene;
		keys.push_back(tag);
	}
	else {
		std::cout << "WARNING - Scene: " << tag << " already exists.\n";
	}

	return scene.scene;
}

Scene * Game::getScene(std::string tag) {
	if (sceneExists(tag)) {
		return scenes[tag].scene;
	}
	else {
		std::cout << "WARNING - Scene: " << tag << " does not exist, returning NULL.\n";
		return NULL;
	}
}

bool Game::sceneExists(std::string tag) {
	return !(scenes.find(tag) == scenes.end());
}

void Game::activateScene(std::string tag) {
	if (sceneExists(tag)) {
		scenes[tag].active = true;
	}
}

void Game::deactivateScene(std::string tag) {
	if (sceneExists(tag)) {
		scenes[tag].active = false;
	}
}

void Game::prepareScene() {
	activeKeys.clear();
	for (std::string key : keys) {
		if (sceneExists(key) && scenes[key].active) {
			scenes[key].scene->initializeGameObjects();
			activeKeys.push_back(key);
		}
	}
}

void Game::tick() {

	int frameStart;
	int elapsedTime;

	frameStart = SDL_GetTicks();

	input->pollEvents();
	running = !(input->quit);

	for (std::string key : activeKeys) {
		if (sceneExists(key)) {
			Scene * scene = scenes[key].scene;
			scene->display->clear();
		}
	}

	

	for (std::string key : activeKeys) {
		if (sceneExists(key)) {
			Scene * scene = scenes[key].scene;
			scene->tick();
		}
	}

	for (std::string key : activeKeys) {
		if (sceneExists(key)) {
			scenes[key].scene->display->draw();
		}
	}

	display->render();

	elapsedTime = SDL_GetTicks() - frameStart;

	if (elapsedTime <= 1000 / FPS) {
		SDL_Delay((1000 / FPS) - elapsedTime);
	}

}