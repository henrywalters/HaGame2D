#pragma once
#include "HaGame2D.h"
#include "Algorithms.h"

const float SIZE = 150;

struct RaycastHit {
	std::vector<GameObject*> hits;
	GameObject* closestHit;
};

class Raycaster : public System {
public:

	int divisions = 32;
	double rayLength = 150;
	std::vector<GameObject*> gameObjects;
	Scene* scene;

	Raycaster() : System("Raycaster") {}

	void onInit() {
		scene = getScene();
		gameObjects = scene->getGameObjectsWhere<CollisionComponent>();
	}

	void update() {

		int divisions = 16;

		for (int i = 0; i < divisions; i++) {

			std::vector<RaycastHit> hits = std::vector<RaycastHit>();
			bool hit = false;

			Vector p1 = scene->input->globalMousePos();
			Vector p2 = scene->input->globalMousePos() + Math::rotateVector(Vector(1.0, 0.0), -M_PI / (divisions / 2.0) * i) * rayLength;

			std::vector<GameObject*> goHits = std::vector<GameObject*>();
			for (auto object : gameObjects) {
				BoxCollider* coll = object->getComponent<BoxCollider>();
				if (coll != NULL) {
					Box box = coll->getBox();
					auto clippedLine = Algorithms::LiangBarsky(box.x, box.y, box.x + box.width, box.y + box.height, p1.x, p1.y, p2.x, p2.y);
					if (clippedLine.has_value()) {
						scene->display->drawLine(clippedLine.value().p1(), clippedLine.value().p2(), Color::red(), 15);
						hit = true;
						goHits.push_back(object);
					}
				}
			}

			if (hit) {
				RaycastHit hit;
				hit.hits = goHits;
				hit.closestHit = goHits[Algorithms::MinIndex<GameObject*, float>(goHits, [&](GameObject* go) {
					return (scene->input->globalMousePos() - go->position).magnitude();
					})];
				hit.closestHit->getComponent<BoxRenderer>()->color = Color::red();
			}
		}
	}
};

class CharacterController : public System {
public:

	float speed = 600;
	float rayLength = 200;

	Scene* scene;
	Input* input;

	std::vector<Vector> raycastFrom = {
		Vector(0, 0),
		Vector(0, SIZE / 2),
		Vector(0, SIZE),
		Vector(SIZE / 2, 0),
		Vector(SIZE / 2, SIZE / 2),
		Vector(SIZE / 2, SIZE),
		Vector(SIZE, 0),
		Vector(SIZE, SIZE / 2),
		Vector(SIZE, SIZE),
	};

	CharacterController() : System("Character Controller") {}

	void onInit() {
		scene = getScene();
		input = scene->input;
	}

	void update() {

		Vector dir = Vector::Zero();
		if (input->up) {
			dir.y = -1;
		}
		if (input->down) {
			dir.y = 1;
		}
		if (input->right) {
			dir.x = 1;
		}
		if (input->left) {
			dir.x = -1;
		}

		scene->camera.move(dir.normalized() * scene->dt_s() * speed);
		
	}
};

class RaycastTest : public Game {
public:

	Scene* test;
	GameObject* player;

	RaycastTest(int width, int height) : Game(width, height, "Raycast Test") {
		test = addScene("test");
		test->display->clearColor = Color::white();
		player = test->add()->setPosition(Vector(50, 50));

		test->addSystem(new CharacterController());
		// test->addSystem(new Raycaster());

		_fps = 30;
		_capFps = false;

		for (int i = 0; i < 150; i++) {
			for (int j = 0; j < 150; j++) {
				addRect(Vector(i * 100, j * 100), Vector(25, 25));
			}
		}


		addRect(Vector(400, 400), Vector(200, 200));
		addRect(Vector(200, 400), Vector(100, 100));
	}

	void run() {
		test->initialize();
		while (running) {
			tick();
		}
	}

	GameObject* addRect(Vector pos, Vector size) {
		return test->add()
			->addComponentAnd(new BoxRenderer(size.x, size.y, true, Color::blue()))
			->addComponentAnd(new BoxCollider(size))
			->setPosition(pos);
	}
};