#include "stdafx.h"
#include "ZeldaClone.h"
#include "HaGame2D.h"
#include "Camera.h"
#include "SpriteAnimationRenderer.h"
#include "Game.h"
#include "MapLoader.h"

char * LINK = "link";


const int tileSize = 50;
const int playerSize = 50;

class Animator : public Component {

	SpriteAnimationRenderer * anim;

	void onCreate() {
		anim = transform->getComponent<SpriteAnimationRenderer>();
	}

	void update() {
		if (input->up) {
			anim->setState("movef");
		}

		else if (input->down) {
			anim->setState("moveb");
		}

		else if (input->left) {
			anim->setState("movel");
		}

		else if (input->right) {
			anim->setState("mover");
		}
		else if (input->space) {
			anim->setState("attack");
		}
		else {
			anim->setState("idle");
		}
	}
};

class MobSimpleController : public Component {
public:
	Vector velocity;
	int lastChange;
	int changeIn;

	MobSimpleController(Vector vel) {
		velocity = vel;
	}

	void onCreate() {
		changeIn = Random::number(0, 3000);
		lastChange = SDL_GetTicks();
	}

	void update() {
		BoxCollider * coll = transform->getComponent<BoxCollider>();
		auto collisions = coll->currentCollisions;

		bool wallCollided = false;
		Box wall;

		auto playerObject = gameScene->getGameObject("link");
		auto player = playerObject->position;
		auto playerBox = playerObject->getComponent<BoxComponent>();

		Vector playerCenter = Vector(player.x + playerBox->width / 2, player.y + playerBox->height / 2);

		Vector playerDelta = transform->position - playerCenter;

		bool chasing = false;

		if (playerDelta.magnitude() < 200) {
			velocity = playerCenter - transform->position;
			velocity.normalize();
			chasing = true;
		}
		else {

			if (SDL_GetTicks() - lastChange > changeIn) {
				changeIn = Random::number(0, 3000);
				lastChange = SDL_GetTicks();
				velocity.x = Random::number(-100, 100);
				velocity.y = Random::number(-100, 100);
				velocity.normalize();

				velocity = velocity * 2;
			}
		}

		for (auto collision : collisions) {
			if (collision.gameObject->tag == "wall" || (collision.gameObject->tag == "squib" && collision.gameObject->id != transform->id)) {
				wallCollided = true;
				wall = collision.gameObject->getComponent<BoxCollider>()->getBox();
			}

		}

		if (wallCollided) {

			Box box = coll->getBox();
			int signX = box.x >= wall.x ? 1 : -1;
			int signY = box.y >= wall.y ? 1 : -1;

			velocity.x = Random::number(1, 100) * signX;
			velocity.y = Random::number(1, 100) * signY;

			velocity.normalize();

			velocity = velocity * 2;
		}

		transform->move(velocity);
	}
};

class Squib {
public:

	static GameObject * initialize(Scene * scene, Vector position, Vector velocity) {

		float squibSize = 25;

		GameObject * squib = scene->add();
		squib->addComponent(new BoxCollider(squibSize, squibSize));
		squib->addComponent(new MobSimpleController(velocity));
		squib->move(position);
		squib->z_index = 15;
		squib->tag = "squib";
		squib->addComponent(new BoxRenderer(squibSize, squibSize, true, Color::blue()));
		return squib;
	}
};

class Link {
public:

	int health = 100;

	bool alive = true;

	void hit(int damage) {
		health -= damage;
		alive = health > 0;
	}

	static GameObject * initialize(Scene *scene) {
		GameObject * link = scene->add();
		link->addComponent(new BoxCollider(playerSize, playerSize));
		auto animator = link->addComponent(new SpriteAnimationRenderer("../Assets/Sprites/HaGameEngine/Characters/Link.png", "LinkSprites.txt"));
		link->addComponent(new OrthogonolCharacterController());
		link->addComponent(new Animator());
		auto linkController = link->getComponent<OrthogonolCharacterController>();
		link->id = "link";
		link->tag = "player";
		linkController->speed = 4;
		link->z_index = 15;
		return link;
	}
};

class Particle : BoxComponent {
	RGB color;
	Vector velocity;
	float speed;
};

ZeldaClone::ZeldaClone()
{

	Game zelda = Game(1000, 800, "Zelda Clone");
	
	Scene overWorld = *zelda.addScene("overworld");
	Scene menu = *zelda.addScene("menu");

	overWorld.setDisplayPort(0, 0, 700, 650);
	menu.setDisplayPort(700, 0, 300, 650);

	auto title = menu.add();
	auto titleTxt = title->addComponent(new TextRenderer(300, 50));
	titleTxt->setMessage("Hathorian");
	titleTxt->setAllignment(TextAllignments::Center);


	auto map = overWorld.add();
	auto bg = overWorld.add();

	auto mapTileDef = SpriteSheetLoader::getSpriteMap("mapbuilder-tiles.txt");
	auto mapTiles = overWorld.display->loadTexture("../Assets/Sprites/HaGameEngine/Environment/mapbuilder-tiles.png");
	auto mapData = MapLoader::load("mapdrawerfile.txt");

	for (MapCell mapCell : mapData) {
		auto mapTile = mapTileDef[mapCell.key];
		auto tile = bg->add();
		tile->addComponent(new SpriteRenderer(mapTiles, mapTile, mapCell.width * tileSize, mapCell.height * tileSize));
		tile->move(Vector(mapCell.y * tileSize, mapCell.x * tileSize));
		tile->staticObject = true;
		if (mapCell.key == "wall" || mapCell.key == "tree") {
			tile->tag = "wall";
			tile->addComponent(new BoxCollider(tileSize, tileSize));
		}
	}

	auto link = Link::initialize(&overWorld);

	Squib::initialize(&overWorld, Vector(500, 275), Vector(0, 2));
	Squib::initialize(&overWorld, Vector(600, 375), Vector(0, 2));
	Squib::initialize(&overWorld, Vector(700, 575), Vector(0, 2));

	Random rand;

	for (int i = 0; i < 100; i++) {
		Vector pos = Vector(rand.generate(100, 500), rand.generate(100, 500));
		Vector vel = Vector(rand.generate(1, 3), rand.generate(1, 3));

		std::cout << "Vector: ";
		pos.display();
		vel.display();
		Squib::initialize(&overWorld, pos, vel);
	}

	link->move(Vector(275, 275));

	zelda.prepareScene();

	while (zelda.running) {
		zelda.tick();
	}

}


ZeldaClone::~ZeldaClone()
{
}
