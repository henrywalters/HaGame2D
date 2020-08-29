#pragma once
#include "HaGame2D.h"

namespace TGM {
	const Vector GRID_SIZE = Vector(1000, 1000);
	const Vector GRID_PARTITIONS = Vector(50, 50);
	const Vector CELL_SIZE = Vector(GRID_SIZE.x / GRID_PARTITIONS.x, GRID_SIZE.y / GRID_PARTITIONS.y);

	const float CAMERA_SLOW = 10.0f;
	const float CAMERA_FAST = 50.0f;

	const float CAMERA_ZOOM_SLOW = 0.1f;
	const float CAMERA_ZOOM_FAST = 0.5f;

	enum BuilderAction {
		Draw,
		Erase,
	};

	class CameraSystem : public System {

		Scene* scene;
		float speed;
		float zoomSpeed;

	public:
		CameraSystem() : System("Camera System") {}

		void onInit() {
			scene = getScene();

			scene->camera.position(Vector((-GRID_PARTITIONS.x / 2.0) * CELL_SIZE.x, (-GRID_PARTITIONS.y / 2.0) * CELL_SIZE.y));
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

			if (scene->input->one) {
				scene->camera.zoomIn(zoomSpeed * dt);
			}

			if (scene->input->two) {
				scene->camera.zoomIn(-zoomSpeed * dt);
			}
			
		}
	};

	class DrawingSystem : public System {
	private:
		GridLines* gridLines;
		EventManager<Vector>* gridEvents;
		Scene* scene;
		Vector mousePos;
		Vector index;

	public:
		DrawingSystem(GridLines* _gridLines, EventManager<Vector>* _gridEvents) : System("Drawing System") {
			gridLines = _gridLines;
			gridEvents = _gridEvents;
		}

		void onInit() {
			scene = getScene();
		}

		void update() {
			mousePos = scene->input->globalMousePos();
			index = Vector(
				floor((mousePos.x - gridLines->transform->position.x) / CELL_SIZE.x),
				floor((mousePos.y - gridLines->transform->position.y) / CELL_SIZE.y)
			);
			
			hover();

			if (scene->input->fire1) {
				gridEvents->emit("draw", index);
			}

			if (scene->input->fire2) {
				gridEvents->emit("erase", index);
			}
		}

		void hover() {
			auto scale = gridLines->getRelativeScale();
			auto size = Vector(CELL_SIZE.x * scale.x, CELL_SIZE.y * scale.y);
			auto pos = Vector(1 + gridLines->transform->relativePosition.x + size.x * index.x, 1 + gridLines->transform->relativePosition.y + size.y * index.y);
			scene->display->fillRect(pos.x, pos.y, size.x, size.y, Color::blue());
		}
	};

	class LevelBuilder : public Game {

		Scene* builder;
		Scene* toolkit;

		EventManager<Vector> *gridEvents;
	public:
		LevelBuilder() : Game(1000, 800, "The Great Machine: Level Builder") {

			gridEvents = new EventManager<Vector>();

			gridEvents->on("draw", [](Vector v) {
				std::cout << "Draw: " << v.toString() << std::endl;
			});

			gridEvents->on("erase", [](Vector v) {
				std::cout << "Erase: " << v.toString() << std::endl;
			});

			builder = addScene("builder");
			toolkit = addScene("toolkit");

			auto grid = builder->add()
				->setPosition(Vector(-1 * GRID_SIZE.x, -1 * GRID_SIZE.y))
				->addComponentAnd(new GridLines(GRID_SIZE.x, GRID_SIZE.y, GRID_PARTITIONS.x, GRID_PARTITIONS.y));

			builder->addSystem(new CameraSystem());
			builder->addSystem(new DrawingSystem(grid->getComponent<GridLines>(), gridEvents));
		}

		void run() {
			builder->initialize();
			toolkit->initialize();

			while (running) {
				tick();
			}
		}
	};
}