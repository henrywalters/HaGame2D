#pragma once
#include "HaGame2D.h"
#include "Matrix.h"
#include "RigidBody.h"

// Game width & height
const int G_WIDTH = 600;
const int G_HEIGHT = 500;

// Player width & height
const int P_WIDTH = 30;
const int P_HEIGHT = 50;

// Platform default width & heights
const int PF_WIDTH = 200;
const int PF_HEIGHT = 30;

const RGB PF_COLOR = Color::blue();

const std::vector<Box> LEVEL_1 = {
	Box{ 0, G_HEIGHT - 2 * PF_HEIGHT, G_WIDTH * 5, PF_HEIGHT },
	Box{ 0, G_HEIGHT - 4 * PF_HEIGHT, P_WIDTH, PF_HEIGHT}
};

class Player {
	GameObject* player;

public:

	Player(Scene* scene, Vector position) {
		player = scene->add();
		player
			->setName("Player")
			->addComponentAnd(new RigidBody())
			->addComponentAnd(new BoxCollider(P_WIDTH, P_HEIGHT))
			->addComponentAnd<BoxRenderer>(new BoxRenderer(P_WIDTH, P_HEIGHT), [](BoxRenderer* box) {
				box->fill = true;
				box->color = Color::green();
				})
			->addComponentAnd(new VerticalCharacterController())
			->setPosition(position);
	}

	Player(Scene *scene) {
		Player(scene, Vector::Zero());
	}
};

class Platform {
	GameObject* platform;
	Box dimensions;
	RGB color;
public:
	
	Platform(Scene* scene, Box platformDimensions, RGB platformColor) {
		color = platformColor;
		dimensions = platformDimensions;

		platform = scene->add();

		platform
			->setName("Platform")
			->addComponentAnd<BoxRenderer>(new BoxRenderer(dimensions.width, dimensions.height), [](BoxRenderer* box) {
				box->color = Color::blue();
				box->fill = true;
			})
			->addComponentAnd(new BoxCollider(dimensions.width, dimensions.height))
			->setPosition(Vector(dimensions.x, dimensions.y));
	}

	Platform(Scene* scene, Box platformDimensions) {
		Platform(scene, platformDimensions, PF_COLOR);
	}
};

class Platformer
{
public:
	
	Platformer() {
		Game game = Game(G_WIDTH, G_HEIGHT, "HaPlatformer");
		Scene* menu = game.addScene("menu", false);
		Scene* world = game.addScene("world");
		Scene* console = game.addScene("console");

		world->setDisplayPort(0, 0, G_WIDTH, G_HEIGHT);
		menu->setDisplayPort(0, 0, G_WIDTH, G_HEIGHT);
		
		Player player = Player(world, Vector(G_WIDTH / 2 - P_WIDTH / 2, G_HEIGHT / 2 - P_WIDTH / 2));

		std::vector<Platform> platforms;

		for (Box pf : LEVEL_1) {
			platforms.push_back(Platform(world, pf));
		}

		game.prepareScene();

		while (game.running) {
			game.tick();
		}
	}
};

