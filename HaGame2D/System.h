#pragma once
#include <string>
#include "Scene.h"

class System
{
private:

	std::string _name;
	Scene* _scene;

public:

	System(std::string sysName) {
		_name = sysName;
	}

	std::string getName() {
		return _name;
	}

	void setScene(Scene *scene) {
		_scene = scene;
	}

	Scene* getScene() {
		return _scene;
	}

	virtual void update() {};
	virtual void onInit() {};
};

