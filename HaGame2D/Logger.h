#pragma once
#include <string>

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

	virtual void log(std::string message) {};
};

