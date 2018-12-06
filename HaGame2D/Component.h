#pragma once
#include "Vector.h"
#include "Display.h"
#include "Input.h"

class GameObject;
class Scene;

class Component
{
public:

	typedef Component base;
	
	bool active = true;

	Display *display;
	Input *input;

	Component();
	virtual ~Component();

	GameObject * transform;
	Scene * gameScene;


	virtual void onCreate() {};
	virtual void onDestroy() {};
	virtual void update() {};

	void initialize(Display * _display, Input * _input, Scene * _gameScene) {
		display = _display;
		input = _input;
		gameScene = _gameScene;
	}

	template<class T>
	bool isInstanceOf() {
		return dynamic_cast<T *>(this) != NULL;
	}
};

