#pragma once
#include "Vector.h"
#include "Display.h"
#include "Input.h"
#include "Logger.h"

class GameObject;
class Scene;

class Component
{
private:
	Logger *logger;
	bool hasLogger = false;
public:

	typedef Component base;
	
	bool active = true;

	Display *display;
	Input *input;


	Component();

	virtual ~Component() {
		active = false;
		onDestroy();
	};

	GameObject * transform;
	Scene * gameScene;

	virtual void onCreate() {};
	virtual void onDestroy() {};
	virtual void update() {};

	void initialize(Display * _display, Input * _input, Scene *_gameScene) {
		display = _display;
		input = _input;
		gameScene = _gameScene;
	}

	void setLogger(Logger *_logger) {
		logger = _logger;
		hasLogger = true;
	}


	void log(std::string message) {
		if (hasLogger || logger != NULL) {
			logger->log(message);
		}
		else {
			//std::cout << "WARNING - can not log message because no logger has been set\n";
		}
	}

	template<class T>
	bool isInstanceOf() {
		return dynamic_cast<T *>(this) != NULL;
	}
};

