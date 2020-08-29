#pragma once
#include "HaGame2D.h"
#include "System.h"

const Vector GRID_SIZE = Vector(1000, 1000);

const float CAMERA_SLOW = 10.0f;
const float CAMERA_FAST = 50.0f;

const float CAMERA_ZOOM_SLOW = 0.5f;
const float CAMERA_ZOOM_FAST = 5.0f;

class CameraSystem : public System {

	Scene* scene;
	float speed;
	float zoomSpeed;

public:
	CameraSystem() : System("Camera System") {}

	void onInit() {
		scene = getScene();
	}

	void update() {

		float dt = scene->dt_s();

		speed = scene->input->shift ? CAMERA_FAST : CAMERA_SLOW;
		zoomSpeed = scene->input->shift ? CAMERA_ZOOM_FAST : CAMERA_ZOOM_SLOW;

		if (scene->input->up) {
			scene->camera.move(Vector(0, -speed * dt));
		}

		if (scene->input->down) {
			scene->camera.move(Vector(0, speed * dt));
		}

		if (scene->input->left) {
			scene->camera.move(Vector(-speed * dt, 0));
		}

		if (scene->input->right) {
			scene->camera.move(Vector(speed * dt, 0));
		}

		scene->camera.zoomIn(zoomSpeed * scene->input->mouseScroll * dt);
	}
};

class PlatformerMapBuilder : public Game {
private:

	Scene* view;
	Scene* toolbox;

public:

	PlatformerMapBuilder() : Game(GRID_SIZE.x, GRID_SIZE.y, "Platformer Map Builder") {
		view = addScene("view");
		toolbox = addScene("toolbox");

		view->addSystem(new CameraSystem());

		view->add()
			->setPosition(Vector(-1 * GRID_SIZE.x, -1 * GRID_SIZE.y))
			->addComponent(new GridLines(GRID_SIZE.x * 2, GRID_SIZE.y * 2, 50, 50));
	}

	void run() {

		view->initialize();
		toolbox->initialize();

		while (running) {
			tick();
		}
	}
};