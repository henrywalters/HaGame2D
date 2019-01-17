#include "stdafx.h"
#include "MapBuilder.h"
#include "Component.h"
#include "GameObject.h"
#include "Component.h"
#include "Grid.h"
#include "TextRenderer.h"
#include "Scene.h"
#include "Game.h"
#include "GridDrawer.h"
#include "ButtonComponent.h"
#include "Input.h"
#include <functional>

class CamController : public Component {

	Vector camPos = Vector::Zero();
	float camSpeed = 5;

	void update() {
		camPos = Vector::Zero();

		if (input->up) {
			camPos.y = -1 * camSpeed;
		}
		else if (input->down) {
			camPos.y = camSpeed;
		}

		if (input->left) {
			camPos.x = -1 * camSpeed;
		}
		else if (input->right) {
			camPos.x = camSpeed;
		}

		if (input->one) {
			gameScene->camera.zoomIn();
		}

		if (input->two) {
			gameScene->camera.zoomOut();
		}

		gameScene->camera.move(camPos);
	}
};

class TileButton : public ButtonComponent {
public :

	TileButton(float width, float height) : ButtonComponent(width, height) {

	}

	Texture spriteSheet;
	SpriteSheetCell cell;
	std::function<void(PalletItem *)> onClickFunc;

	PalletItem * getPalletItem() {
		PalletItem * item = new PalletItem();
		item->spriteSheet = spriteSheet;
		item->spriteSheetCell = cell;
		return item;
	}

	void onClick() {
		onClickFunc(getPalletItem());
	}
};

class SelectLayerButton : public ButtonComponent {
public:
	int z_index;

	std::function<void(int)> onClickFunc;

	SelectLayerButton(float width, float height, int layer) : ButtonComponent(width, height) {
		std::cout << "Layer: " << layer << "\n";
		z_index = layer;
	};

	void onClick() {
		onClickFunc(z_index);
	}
};

class ViewLayerButton : public ButtonComponent {
public:
	int z_index;
	bool active = true;

	std::function<void(int, bool)> onClickFunc;

	ViewLayerButton(float width, float height, int layer) : ButtonComponent(width, height) {
		z_index = layer;
	};

	~ViewLayerButton() {};

	void onClick() {
		active = !active;
		onClickFunc(z_index, active);
	}

};

template <class T>
class ValueButtonComponent : public ButtonComponent {
	T _value;
public:
	std::function<void(T)> onClickFunc;

	ValueButtonComponent(float width, float height, T value) : ButtonComponent(width, height) {
		_value = value;
	}

	~ValueButtonComponent() {};

	void onClick() {
		onClickFunc(_value);
	}
};

class Btn {
public:
	static GameObject * initialize(Scene *scene, Vector pos, Vector size, std::string msg, RGB color = Color::black()) {
		auto btn = scene->add();
		btn->move(pos);
		auto btnComp = btn->addComponent(new ButtonComponent(size.x, size.y));
		auto text = btn->addComponent(new TextRenderer(size.x, size.y));
		text->setFontSize(16);
		text->setMessage(msg);
		text->setFontColor(color);
		text->setAllignment(TextAllignments::Center);
		return btn;
	}
};

template <class T>
class ValueBtn {
public:
	static GameObject * initialize(Scene *scene, Vector pos, Vector size, std::string msg, T value, RGB color = Color::black()) {
		auto btn = scene->add();
		btn->move(pos);
		auto btnComp = btn->addComponent(new ValueButtonComponent<T>(size.x, size.y, value));
		auto text = btn->addComponent(new TextRenderer(size.x, size.y));
		text->setFontSize(16);
		text->setMessage(msg);
		text->setFontColor(color);
		text->setAllignment(TextAllignments::Center);
		return btn;
	}
};

class Label {
public:
	static GameObject * initialize(Scene *scene, Vector pos, Vector size, std::string msg, RGB color = Color::black()) {
		auto text = scene->add();
		text->move(pos);
		auto renderer = text->addComponent(new TextRenderer(size.x, size.y));
		renderer->setAllignment(TextAllignments::Center);
		renderer->setFontSize(16);
		renderer->setFontColor(color);
		renderer->setMessage(msg);
		return text;
	}
};

MapBuilder::MapBuilder(char * save)
{
	saveFile = save;
	int mapWidth = 4000;
	int mapHeight = 4000;
	int rows = 75, cols = 75;


	const int layers = 5;

	const int screenWidth = 1800;
	const int screenHeight = 900;

	const int worldWidth = 1500;
	const int worldHeight = 700;

	const int palletWidth = screenWidth;
	const int palletHeight = screenHeight - worldHeight;
	const RGB palletColor = Color(61, 61, 61).rgb;

	const int toolWidth = screenWidth - worldWidth;
	const int toolHeight = worldHeight;
	const RGB toolColor = Color::parseHex("939393");

	Game builder = Game(screenWidth, screenHeight, "Map Builer: Untitled.map");

	SDL_DisplayMode dm;
	SDL_GetDesktopDisplayMode(0, &dm);

	auto w = dm.w;
	auto h = dm.h;

	printf("Screen height %f & width %f ", w, h);

	SDL_SetWindowFullscreen(builder.display->window, 0);

	Scene world = *builder.addScene("world");
	Scene pallet = *builder.addScene("pallet");
	Scene tools = *builder.addScene("tools");

	auto tileSheet = pallet.display->loadTexture("../Assets/Sprites/HaGameEngine/Environment/platformer-tiles.png");
	auto tiles = SpriteSheetLoader::load("../Assets/SSDefinitions/platformer-tiles.ssd");


	world.setDisplayPort(0, 0, worldWidth, worldHeight);
	pallet.setDisplayPort(0, worldHeight, palletWidth, palletHeight);
	tools.setDisplayPort(worldWidth, 0, toolWidth, toolHeight);

	auto builderGrid = world.add();
	builderGrid->move(Vector(-0.5 * mapWidth, -0.5 * mapHeight));
	builderGrid->add()->addComponent(new GridLines(mapWidth, mapHeight, cols, rows));
	auto map = builderGrid->add();

	GridDrawer *mapDrawer = map->addComponent(new GridDrawer(mapWidth, mapHeight, rows, cols));

	auto palletBg = pallet.add();
	palletBg->z_index = 1;
	palletBg->addComponent(new BoxRenderer(palletWidth, palletHeight, true, palletColor));

	auto toolBg = tools.add();
	toolBg->z_index = 1;
	toolBg->addComponent(new BoxRenderer(toolWidth, toolHeight, true, toolColor));

	Label::initialize(&tools, Vector(0, 0), Vector(toolWidth, 50), "Map Builder");

	Btn::initialize(&tools, Vector(10, 50), Vector(80, 50), "Save")->getComponent<ButtonComponent>()->onClickFunc = [mapDrawer, save]() {
		mapDrawer->save(save);
	};

	Btn::initialize(&tools, Vector(100, 50), Vector(80, 50), "Load")->getComponent<ButtonComponent>()->onClickFunc = [mapDrawer, tileSheet, save]() {
		std::cout << "Loading map: " << save << "\n";
		mapDrawer->tryLoad(save, tileSheet);
	};

	Btn::initialize(&tools, Vector(190, 50), Vector(80, 50), "Clear", Color::red())->getComponent<ButtonComponent>()->onClickFunc = [mapDrawer]() {
		mapDrawer->clearAll();
	};

	Label::initialize(&tools, Vector(10, 125), Vector(180, 35), "Layer Controls");

	Label::initialize(&tools, Vector(190, 125), Vector(toolWidth - 180, 35), "Brush Size");

	for (int i = 0; i < layers; i++) {

		ValueBtn<int>::initialize(&tools, Vector(100, 160 + i * 60), Vector(80, 50), std::to_string(i), i)->getComponent<ValueButtonComponent<int>>()->onClickFunc = [mapDrawer](int layer) {
			mapDrawer->setLayer(layer);
			std::cout << "Selected layer: " << layer << "\n";
		};

		ValueBtn<int>::initialize(&tools, Vector(10, 160 + i * 60), Vector(80, 50), "Hide", i)->getComponent<ValueButtonComponent<int>>()->onClickFunc = [mapDrawer](int layer) {
			mapDrawer->toggle(layer);
			std::cout << "Selected layer: " << layer << "\n";
		};

		
	}

	ValueBtn<int>::initialize(&tools, Vector(210, 160), Vector(80, 40), "1", 1)->getComponent<ValueButtonComponent<int>>()->onClickFunc = [mapDrawer](int size) {
		mapDrawer->setBrushSize(size);
	};

	for (int i = 1; i < 10; i++) {
		ValueBtn<int>::initialize(&tools, Vector(210, 160 + i * 45), Vector(80, 40), std::to_string(i * 2), i * 2)->getComponent<ValueButtonComponent<int>>()->onClickFunc = [mapDrawer](int size) {
			mapDrawer->setBrushSize(size);
		};
	}

	//mapDrawer = mapLayers[0];

	const int tileSize = 20;
	const int rowSize = 10;

	int offsetX = 0;
	int offsetY = 0;

	for (int i = 0; i < tiles.size(); i++) {
		auto palletTile = pallet.add();
		palletTile->addComponent(new SpriteRenderer(tileSheet, tiles[i], tileSize, tileSize));

		if (i % rowSize == 0) {
			offsetX = 0;
			offsetY += tileSize;
		}
		else {
			offsetX += tileSize;
		}

		palletTile->move(Vector(offsetX, offsetY));
		auto palletTileBtn = palletTile->addComponent(new TileButton(tileSize, tileSize));
		palletTileBtn->spriteSheet = tileSheet;
		palletTileBtn->cell = tiles[i];
		palletTileBtn->onClickFunc = [mapDrawer](PalletItem * pallet) {
			mapDrawer->setPallet(pallet->spriteSheet, pallet->spriteSheetCell);
		};
		/*
		palletTileBtn->onHoverFunc = [pallet, palletTileBtn]() {
			Vector pos = palletTileBtn->transform->position;
			Vector size = Vector(palletTileBtn->cell.width, palletTileBtn->cell.height);
			pallet.display->drawRect(pos.x, pos.y, size.x, size.y, Color::blue());
		};
		*/
	}


	auto cam = world.add()->addComponent(new CamController());
	world.camera.move(Vector(worldWidth / 2, worldHeight / 2));

	builder.prepareScene();

	while (builder.running) {
		builder.tick();
	}

	/*Scene builder = *game.addScene("builder");
	Scene pallet = *game.addScene("pallet-ui-1");
	Scene pallet2 = *game.addScene("pallet-ui-2");

	builder.setDisplayPort(0, 0, game.screenWidth, game.screenHeight - 100);
	pallet.setDisplayPort(0, game.screenHeight - 200, game.screenWidth, 100);
	pallet2.setDisplayPort(0, game.screenHeight - 100, game.screenWidth, 100);

	for (int i = 0; i < 25; i++) {
		auto tile = pallet.add();
		tile->addComponent(new BoxRenderer(50, 100, true, Color(0, 0, i * 10).rgb));
		tile->move(Vector(i * 50, 0));

		tile = pallet2.add();
		tile->addComponent(new BoxRenderer(50, 100, true, Color(0, i * 10, 0).rgb));
		tile->move(Vector(i * 50, 0));
	}

	auto grid = builder.add();
	grid->addComponent(new GridLines(5000, 5000, 50, 50));

	auto drawer = builder.add();
	drawer->addComponent(new GridDrawer(5000, 5000, 50, 50));

	auto cam = builder.add();
	cam->addComponent(new CamController());

	game.prepareScene();

	while (game.running) {
		game.tick();
	}
	*/
}


MapBuilder::~MapBuilder()
{
}
