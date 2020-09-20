#pragma once
#include "HaGame2D.h"

//By default tick every frame.
const int DEFAULT_LOOP_DELAY = 1;

class LoopComponent : public Component {
protected:
	int delay;
	int lastTick;
	bool canTick = true;

	virtual void tick() = 0;

public:

	LoopComponent(int delayInMs) {
		delay = delayInMs;
	}

	void onCreate() {
		lastTick = SDL_GetTicks();
	}

	void update() {
		if (canTick) {
			if (SDL_GetTicks() - lastTick >= delay) {
				lastTick = SDL_GetTicks();
				std::cout << "Ticking\n";
				tick();
			}
		}
		else {
			gameScene->destroy(transform);
		}
	}
};

class ForLoop : public LoopComponent {

	std::function<void(int)> callback;
	int begin, end;
	double step;
	int index;

protected:

	void tick() {
		if (index * step + begin >= end) {
			canTick = false;
		}
		else {
			callback(index);
			index++;
		}
	}

public:

	ForLoop(int _begin, int _end, double _step, int delayInMs, std::function<void(int)> _callback) : LoopComponent(delayInMs) {
		begin = _begin, end = _end, step = _step;
		callback = _callback;
		index = 0;
	}
};

class WhileLoop : public LoopComponent {
	std::function<void()> callback; 
	std::function<bool()> conditional;
protected:

	void tick() {
		if (conditional()) {
			tick();
		}
		else {
			canTick = false;
		}
	}

public:
	
	WhileLoop(int delayInMs, std::function<bool()> _conditional, std::function<void()> _callback) : LoopComponent(delayInMs) {
		callback = _callback;
		conditional = _conditional;
	}
};

class Loop
{
public:
	Loop();
	~Loop();

	static GameObject * For(Scene *scene, int begin, int end, double step, int delayInMs, std::function<void(int)> callback) {
		auto loopObject = scene->add();
		loopObject->addComponent(new ForLoop(begin, end, step, delayInMs, callback));
		scene->instantiate(loopObject);
		return loopObject;
	}
};

