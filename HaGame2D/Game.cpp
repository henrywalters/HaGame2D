#include "stdafx.h"
#include "Game.h"


Game::Game(int _screenWidth, int _screenHeight, char * _title)
{
	title = _title;
	screenWidth = _screenWidth; screenHeight = _screenHeight;
	display = new Display(screenWidth, screenHeight, title);
	display->setFont(24, DOROVAR_FONT);
	input = new Input();
	setFps(FPS, CAP_FPS);
}

Game::~Game()
{
}

void Game::setFps(int fps, bool fpsCapped)
{
	_fps = fps;
	_capFps = fpsCapped;
	maxTick = 1000000 / _fps;
	std::cout << _fps << " " << _capFps;
	std::cout << std::to_string(maxTick);
	fpsMeter = DataSample<float>("FPS", _capFps ? _fps : 100);
	fpsMeter.onFullSample([this](float fpsSample) {
		display->displayMetric("FPS: " + std::to_string(1000000 / fpsSample));
	});
}

Scene * Game::addScene(std::string tag, bool active) {
	GameScene scene;
	scene.active = active;
	scene.scene = new Scene();
	scene.scene->initialize(screenWidth, screenHeight, display, input);
	scene.scene->setDisplayPort(0, 0, screenWidth, screenHeight);

	if (!sceneExists(tag)) {
		scenes[tag] = scene;
		sceneStates[tag] = active;
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
		std::cout << "Activating Scene: " << tag << "\n";
		scenes[tag].active = true;
		sceneStates[tag] = true;
	}
}

void Game::deactivateScene(std::string tag) {
	if (sceneExists(tag)) {
		std::cout << "Deactivating Scene: " << tag << "\n";
		scenes[tag].active = false;
		sceneStates[tag] = false;
	}
}

void Game::resetScene(std::string tag)
{
	if (sceneExists(tag)) {
		scenes[tag].scene->reset();
	}
}

void Game::prepareScene() {
	
	/*
		When building a ui state machine, it is likely you will have multiple scenes with buttons on them.
		Since changing a scene resets the inputs, pressed keys/mouse buttons will be considered up and
		therefore allowed to click. This presents problems. An easy solution is to just poll the events before
		the scene starts to ensure every thing is representative of the true state without affecting game
		state.
	*/

	input->pollEvents();

	for (std::string key : keys) {
		if (sceneExists(key)) {
			if (loggerHandle != NULL) {
				scenes[key].scene->setLogger(loggerHandle);
			}
			else {
				std::cout << "WARNING - failed to set logger handle, falling back to console\n";
				scenes[key].scene->setLogger(new Logger);
			}
			
			scenes[key].scene->initializeGameObjects();
		}
	}
}

void Game::tick() {

	clock.start();

	input->pollEvents();
	running = !(input->quit);

	std::vector<std::string> activeKeys = getActiveScenes();
	
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

	float elapsedTime = clock.stop();

	if (elapsedTime <= 100000 / _fps && _capFps) {
		clock.wait((100000 / _fps) - elapsedTime);
	}

	float dt = clock.stop();

	fpsMeter.add(dt);

	for (std::string key : activeKeys) {
		if (sceneExists(key)) {
			scenes[key].scene->setDt(dt);
		}
	}
}