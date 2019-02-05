#pragma once
#include "HaGame2D.h"



class Controller : public Component
{
	enum ControllerFlag {
		Empty,
		Simple,
		Full
	};

	std::function<void()> _onStartFunc;
	std::function<void()> _onDestroyFunc;
	std::function<void()> _updateFunc;

	ControllerFlag flag;
public:

	Controller() {
		flag = ControllerFlag::Empty;
	}

	Controller(std::function<void()> updateFunc) {
		flag = ControllerFlag::Simple;
		_updateFunc = updateFunc;
	}

	Controller(std::function<void()> onStartFunc, std::function<void()> onDestroyFunc, std::function<void()> updateFunc) {
		flag = ControllerFlag::Full;
		_onStartFunc = onStartFunc;
		_onDestroyFunc = onDestroyFunc;
		_updateFunc = updateFunc;
	}
	~Controller();

	void update() {
		if (flag >= ControllerFlag::Simple) {
			_updateFunc();
		}
	}

	void onCreate() {
		if (flag >= ControllerFlag::Full) {
			_onStartFunc();
		}
	}

	void onDestroy() {
		if (flag >= ControllerFlag::Full) {
			_onDestroyFunc();
		}
	}
};

class ControllerFactory {
public:
	static GameObject * create(Scene * scene, std::function<void()> updateFn) {
		GameObject * emptyObject = new GameObject();
		emptyObject->addComponent(new Controller(updateFn));
		scene->instantiate(emptyObject);
		return emptyObject;
	}

	static GameObject * create(Scene * scene, std::function<void()> updateFn, std::function<void()> onCreateFn, std::function<void()> onDestroyFn) {
		GameObject * emptyObject = new GameObject();
		emptyObject->addComponent(new Controller(updateFn, onCreateFn, onDestroyFn));
		scene->instantiate(emptyObject);
		return emptyObject;
	}
};