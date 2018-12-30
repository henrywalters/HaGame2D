#pragma once
#include <string>
#include "Loggable.h"
#include <iostream>

class Scene;

class Logger
{

public:
	Scene *scene;

	Logger();
	~Logger();

	virtual void setScene(Scene *_scene) {
		scene = _scene;
	}

	virtual void log(std::string message) {
		std::cout << message << "\n";
	};

	virtual void log(Loggable message) {
		log(message.toString());
	}
};

