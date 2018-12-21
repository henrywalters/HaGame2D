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

const int DEFAULT_MIN = 0;
const int DEFAULT_MAX = 100;

class Bar : public BoxComponent {

	RGB color;
	float min, max;
	float current = 50;

public:


	Bar(RGB _color, float _min, float _max, float _current) {
		width = 250;
		height = 25;
		color = _color;
		min = _min;
		max = _max;
		current = _current;
	}

	void setValue(float value) {
		if (value >= min && value <= max) {
			current = value;
		}
	}

	void decrement(float delta) {
		if (current - delta >= min) {
			current = current - delta;
		}
		else {
			current = 0;
		}
	}

	void increment(float delta) {
		if (current + delta <= max) {
			current = current + delta;
		}
		else {
			current = max;
		}
	}

	void update() {
		display->drawRect(transform->relativePosition.x, transform->relativePosition.y, relativeWidth, relativeHeight, color, 15);
		float barWidth = (current / (max - min)) * relativeWidth;
		display->fillRect(transform->relativePosition.x, transform->relativePosition.y, barWidth, relativeHeight, color, 15);
	}
};

class HealthController : public Component {
	float health = 100;
	float healthPot = 25;
	void update() {
		auto collider = transform->getComponent<BoxCollider>();
		for (auto collision : collider->currentCollisions) {
			if (collision.gameObject->tag == "health") {
				health += healthPot;
				healFunc(healthPot);
				gameScene->destroy(collision.gameObject);
			}
		}
	}
public:

	std::function<void(float)> damageFunc;
	std::function<void(float)> healFunc;

	void damage(float dmg) {
		health -= dmg;
		if (damageFunc != NULL) {
			damageFunc(dmg);
		}
	}

	void heal(float hp) {
		health += hp;
		if (healFunc != NULL) {
			healFunc(hp);
		}
	}
};

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

	float damage = 5;

	bool canDamage = true;
	int lastDamage;
	int cooloff = 200;

	MobSimpleController(Vector vel) {
		velocity = vel;
	}

	void onCreate() {
		changeIn = Random::number(0, 3000);
		lastChange = SDL_GetTicks();
	}

	void update() {

		if (!canDamage) {
			if (SDL_GetTicks() - lastDamage > cooloff) {
				canDamage = true;
			}
		}

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

			if (collision.gameObject->tag == "player" && canDamage) {
				auto healthController = gameScene->getGameObject("link")->getComponent<HealthController>();
				if (healthController != NULL) {
					healthController->damage(damage);
				}
				canDamage = false;
				lastDamage = SDL_GetTicks();
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

	static GameObject * initialize(Scene * scene, SpriteAnimationRenderer *renderer, Vector position, Vector velocity) {

		float squibSize = 35.5;

		GameObject * squib = scene->add();
		squib->addComponent(new BoxCollider(squibSize, squibSize));
		squib->addComponent(new MobSimpleController(velocity));
		squib->move(position);
		squib->z_index = 15;
		squib->tag = "squib";
		//squib->addComponent(new BoxRenderer(squibSize, squibSize, true, Color::blue()));
		squib->addComponent(renderer);
		return squib;
	}
};

const float SPELL_SIZE = 30;

class SpellCast : public BoxComponent {
public:
	Vector velocity;
	SpellCast(Vector _velocity) {
		velocity = _velocity;
	}

	void update() {
		auto boxCollider = transform->getComponent<BoxCollider>();
		transform->move(velocity);

		bool colliding = false;

		for (auto collision : boxCollider->currentCollisions) {
			if (collision.gameObject->tag == "wall") {
				colliding = true;
				gameScene->destroy(transform);
			}

			if (collision.gameObject->tag == "squib") {
				gameScene->destroy(collision.gameObject);
			}
		}
	}
};

class Spell {
public:
	static GameObject * castSpell(Scene * scene, Vector position, Vector velocity) {
		auto spell = scene->add();
		spell->addComponent(new BoxCollider(SPELL_SIZE, SPELL_SIZE));

		spell->setPosition(position);
		spell->z_index = 15;
		spell->tag = "spell";
		//spell->addComponent(new BoxRenderer(SPELL_SIZE, SPELL_SIZE, true, Color::green()));

		auto animator = spell->addComponent(new SpriteAnimationRenderer("../Assets/Sprites/HaGameEngine/Misc/spellsheet.png", "spellsheet.txt"));

		spell->addComponent(new SpellCast(velocity));
		return spell;
	}
};



class SpellController : public Component {
public:

	float cooldown = 250;
	int lastCast;
	bool canCast = true;
	float manaCost = 10;

	float mana = 100;

	float manaPot = 25;

	std::function<void(float)> castFunc;
	std::function<void(float)> regenFunc;

	void update() {

		if (!canCast) {
			if (SDL_GetTicks() - lastCast >= cooldown) {
				canCast = true;
			}
		}

		if (input->fire1 && canCast && mana >= manaCost) {
			auto controller = transform->getComponent<OrthogonolCharacterController>();
			auto spell = gameScene->instantiate(Spell::castSpell(gameScene, transform->position, controller->currentDirection * 3));
			auto anim = spell->getComponent<SpriteAnimationRenderer>();
			anim->setState("freeze");
			anim->setStateLength("freeze", .6);
			castFunc(manaCost);
			lastCast = SDL_GetTicks();
			mana -= manaCost;
			canCast = false;
		}

		auto collider = transform->getComponent<BoxCollider>();
		for (auto collision : collider->currentCollisions) {
			if (collision.gameObject->tag == "mana") {
				mana += manaPot;
				regenFunc(manaPot);
				gameScene->destroy(collision.gameObject);
			}
		}
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
		auto spellController = link->addComponent(new SpellController());
		auto healthController = link->addComponent(new HealthController());
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

	auto healthBarContainer = menu.add();
	auto healthBar = healthBarContainer->addComponent(new Bar(Color::red(), 0, 100, 100));
	healthBarContainer->move(Vector(25, 50));

	auto manaBarContainer = menu.add();
	auto manaBar = manaBarContainer->addComponent(new Bar(Color::blue(), 0, 100, 100));
	manaBarContainer->move(Vector(25, 100));


	auto map = overWorld.add();
	auto bg = overWorld.add();

	auto mapTileDef = SpriteSheetLoader::getSpriteMap("mapbuilder-tiles.txt");
	auto mapTiles = overWorld.display->loadTexture("../Assets/Sprites/HaGameEngine/Environment/mapbuilder-tiles.png");
	auto mapData = MapLoader::load("testing-arena.map");

	for (MapCell mapCell : mapData) {
		auto mapTile = mapTileDef[mapCell.key];
		auto tile = bg->add();
		tile->addComponent(new SpriteRenderer(mapTiles, mapTile, mapCell.width * tileSize, mapCell.height * tileSize));
		tile->move(Vector(mapCell.y * tileSize, mapCell.x * tileSize));
		
		if (mapCell.key == "wall" || mapCell.key == "tree") {
			tile->tag = "wall";
			tile->addComponent(new BoxCollider(tileSize, tileSize));
			tile->staticObject = true;
		}

		if (mapCell.key == "health") {
			tile->tag = "health";
			tile->addComponent(new BoxCollider(tileSize, tileSize));
			tile->staticObject = true;
		}

		if (mapCell.key == "mana") {
			tile->tag = "mana";
			tile->addComponent(new BoxCollider(tileSize, tileSize));
			tile->staticObject = true;
		}

		if (mapCell.key == "squib") {
			tile->tag = "squib";
			tile->addComponent(new MobSimpleController(Vector(0, 0)));
			tile->addComponent(new BoxCollider(tileSize, tileSize));
			//tile->addComponent(new BoxRenderer(tileSize, tileSize));
			tile->addComponent(new SpriteRenderer(mapTiles, mapTileDef["squib"], tileSize, tileSize));
			tile->trackedOffScreen = false;
			tile->z_index = 15;
		}

	}

	auto link = Link::initialize(&overWorld);

	auto spellController = link->getComponent<SpellController>();
	spellController->castFunc = [manaBar](float manaCost) {
		manaBar->decrement(manaCost);
	};
	
	spellController->regenFunc = [manaBar](float regen) {
		manaBar->increment(regen);
	};

	auto healthController = link->getComponent<HealthController>();
	healthController->damageFunc = [healthBar](float damage) {
		healthBar->decrement(damage);
	};

	healthController->healFunc = [healthBar](float hp) {
		std::cout << "Played Healed\n";
		healthBar->increment(hp);
	};

	Random rand;

	link->move(Vector(275, 275));

	zelda.prepareScene();

	while (zelda.running) {
		zelda.tick();
	}

}


ZeldaClone::~ZeldaClone()
{
}
