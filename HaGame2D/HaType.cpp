#pragma once
#include "stdafx.h"
#include "HaGame2D.h"
#include "Component.h"
#include "GameObject.h"
#include "Component.h"
#include "HaType.h"
#include "Game.h"

class Bullet : public Component {

	const float height = 2;
	const float width = 5;
	const float speed = 5;

	Vector position;
	Vector velocity = Vector(speed, 0);

	GameObject * self;

	void onCreate() {
		self = gameScene->add();
		self->addComponent(new BoxCollider(width, height));
	}

	void onDestroy() {
		velocity = Vector::Zero();
	}

	void update() {
		transform->move(velocity.normalized() * speed);
		display->drawRect(transform->position.x, transform->position.y, width, height, Color::green(), 15);

		if (transform->position.x > gameScene->screenWidth) {
			gameScene->destroy(transform);
		}
	}
};


class ShipController : public Component {

	SpriteAnimationRenderer * anim;

	bool canFire = true;
	int delay = 250;
	int fireTime;

	Vector velocity;

	float accel = .05;
	float deaccel = .07;
	float maxSpeed = 20;
	float minSpeed = 4;

	float maxTurnSpeed = 4;
	float turnAccel = 0.03;
	float turnDeaccel = 0.07;

	void onCreate() {
		anim = transform->getComponent<SpriteAnimationRenderer>();
		velocity = Vector(minSpeed, 0);
	}


	void update() {

		if (!canFire) {
			if (SDL_GetTicks() - fireTime > delay) {
				canFire = true;
			}
		}

		if (input->right) {
			if (velocity.x < maxSpeed) {
				velocity += Vector(accel, 0);
			}

			anim->setState("fly-fast");
		}
		else {
			if (velocity.x > minSpeed) {
				velocity -= Vector(deaccel, 0);
			}

			anim->setState("fly-slow");
		}

		if (input->down) {
			if (velocity.y < maxTurnSpeed) {
				velocity.y += turnAccel;
			}
		}
		else {
			if (velocity.y > 0) {
				velocity.y -= turnDeaccel;
			}
		}

		if (input->up) {
			if (velocity.y > -1 * maxTurnSpeed) {
				velocity.y -= turnAccel;
			}
		}
		else {
			if (velocity.y < 0) {
				velocity.y += turnDeaccel;
			}
		}


		transform->move(velocity);

		if (transform->position.x > gameScene->screenWidth * (3 / 5)) {
			gameScene->camera.move(Vector(velocity.x, 0));
		}

	}
};

int random(int min, int max) {
	int randNum = rand() % (max - min + 1) + min;
	return randNum;
}

class StarController : public Component {
public:
	float size;
	Color color;
	float height;

	StarController(float _size, Color _color, float _height) {
		size = _size;
		height = _height;
		color = _color;
	}

	void update() {
		if (transform->position.x < gameScene->viewport.get(0)) {
			transform->setPosition(Vector(gameScene->viewport.get(0) + gameScene->screenWidth, random(0, height)));
		}
	}
};

class Stars {
public:
	static void addStars(Scene &game, float stars, float width, float height) {
		const float minStarSize = 1;
		const float maxStarSize = 8;

		const int brightnessLevels = 20;

		for (int i = 0; i < stars; i++) {
			int brightness = random(0, brightnessLevels);
			Color starColor = Color(255 - brightness, 255 - brightness, 255 - brightness);
			int size = random(0, maxStarSize);
			int posX = random(0, width - maxStarSize);
			int posY = random(0, height - maxStarSize);

			auto star = game.add();

			star->addComponent(new BoxRenderer(size, size, true, starColor.rgb));
			star->setPosition(Vector(posX, posY));

			star->addComponent(new StarController(size, starColor, height));

			star->z_index = 3;
		}
	}
};


HaType::HaType()
{
	Game hatype;

	auto game = *hatype.addScene("level 1");

	Stars::addStars(game, 100, game.screenWidth, game.screenHeight);

	float spaceSpeed = 0;

	auto spaceTiles = SpriteSheetLoader::getSpriteMap("ship_sprite.txt");

	Texture spaceTexture = game.display->loadTexture("SpaceGameSprites.png");

	auto ship = game.add();

	ship->addComponent(new BoxCollider(76, 50));
	auto anim = ship->addComponent(new SpriteAnimationRenderer("SpaceGameSprites.png", "ship_sprite.txt"));
	
	ship->addComponent(new ShipController());

	//ship->addComponent(new SpriteRenderer("SpaceGameSprites.png", 300, 200, NULL));

	ship->move(game.screenCenter());

	ship->z_index = 15;
	/*
	for (int i = 0; i < 500; i++) {
		auto wall = game.add();
		wall->addComponent(new BoxRenderer(50, 30));
		wall->addComponent(new BoxCollider(50, 30));
		wall->setPosition(Vector(i * 55, 0));
		wall->staticObject = true;

		wall = game.add();
		wall->addComponent(new BoxRenderer(50, 30));
		wall->addComponent(new BoxCollider(50, 30));
		wall->setPosition(Vector(i * 55, game.screenHeight - 30));
		wall->staticObject = true;
	}
	*/
	game.initializeGameObjects();

	anim->setState("fly-slow");

	game.camera.dolly(Vector(spaceSpeed, 0));

	game.loop();
}


HaType::~HaType()
{
}
