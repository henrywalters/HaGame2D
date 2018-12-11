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
	float camSpeed = 2;

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

class SaveButton : public ButtonComponent {
public:
	SaveButton(float width, float height) : ButtonComponent(width, height) {

	}

	std::function<void()> onClickFunc;

	void onClick() {
		onClickFunc();
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


MapBuilder::MapBuilder(char * save)
{
	saveFile = save;
	int mapWidth = 3000;
	int mapHeight = 3000;
	int rows = 100, cols = 100;

	const int layers = 5;

	const int screenWidth = 800;
	const int screenHeight = 600;

	const int worldWidth = 600;
	const int worldHeight = 500;

	const int palletWidth = screenWidth;
	const int palletHeight = screenHeight - worldHeight;
	const RGB palletColor = Color(61, 61, 61).rgb;

	const int toolWidth = screenWidth - worldWidth;
	const int toolHeight = worldHeight;
	const RGB toolColor = Color::parseHex("939393");

	Game builder = Game(screenWidth, screenHeight, "Map Builer: Untitled.map");

	Scene world = *builder.addScene("world");
	Scene pallet = *builder.addScene("pallet");
	Scene tools = *builder.addScene("tools");

	world.setDisplayPort(0, 0, worldWidth, worldHeight);
	pallet.setDisplayPort(0, worldHeight, palletWidth, palletHeight);
	tools.setDisplayPort(worldWidth, 0, toolWidth, toolHeight);


	world.add()->addComponent(new GridLines(mapWidth, mapHeight, cols, rows));
	auto map = world.add();

	GridDrawer *mapDrawer = map->addComponent(new GridDrawer(mapWidth, mapHeight, rows, cols));

	auto palletBg = pallet.add();
	palletBg->z_index = 1;
	palletBg->addComponent(new BoxRenderer(palletWidth, palletHeight, true, palletColor));

	auto toolBg = tools.add();
	toolBg->z_index = 1;
	toolBg->addComponent(new BoxRenderer(toolWidth, toolHeight, true, toolColor));


	auto toolTitle = tools.add()->addComponent(new TextRenderer(toolWidth, 50));
	toolTitle->setMessage("Map Builder");
	toolTitle->setFontColor(Color::black());
	toolTitle->setAllignment(TextAllignments::Center);

	auto newMapBtn = tools.add();
	newMapBtn->move(Vector(10, 50));

	auto btn = newMapBtn->addComponent(new SaveButton(80, 50));
	auto newMapText = newMapBtn->addComponent(new TextRenderer(80, 50));
	newMapText->setFontSize(16);
	newMapText->setMessage("Save Map");
	newMapText->setFontColor(Color::black());
	newMapText->setAllignment(TextAllignments::Center);

	btn->onClickFunc = [mapDrawer, save]() {
		mapDrawer->save(save);
	};

	auto loadMapBtn = tools.add();
	loadMapBtn->move(Vector(100, 50));

	loadMapBtn->addComponent(new ButtonComponent(80, 50));
	auto loadMapText = loadMapBtn->addComponent(new TextRenderer(80, 50));
	loadMapText->setFontSize(16);
	loadMapText->setMessage("Load Map");
	loadMapText->setFontColor(Color::black());
	loadMapText->setAllignment(TextAllignments::Center);


	for (int i = 0; i < layers; i++) {
		auto selectLayer = tools.add();
		selectLayer->move(Vector(100, 125 + i * 60));
		auto selectLayerBtn = selectLayer->addComponent(new SelectLayerButton(80, 50, i));

		auto selectLayerText = selectLayer->addComponent(new TextRenderer(80, 50));
		selectLayerText->setMessage(std::to_string(i));
		selectLayerText->setAllignment(TextAllignments::Center);
		selectLayerText->setFontColor(Color::black());
		selectLayerText->setFontSize(16);

		selectLayerBtn->onClickFunc = [mapDrawer](int layer) {
			mapDrawer->setLayer(layer);
			std::cout << "Selected layer: " << layer << "\n";
		};

		auto toggleLayer = tools.add();
		toggleLayer->move(Vector(10, 125 + i * 60));
		auto toggleLayerBtn = toggleLayer->addComponent(new ViewLayerButton(80, 50, i));
		auto toggleLayerText = toggleLayer->addComponent(new TextRenderer(80, 50));
		toggleLayerText->setMessage("Hide");
		toggleLayerText->setFontColor(Color::black());
		toggleLayerText->setAllignment(TextAllignments::Center);
		toggleLayerText->setFontSize(16);

		toggleLayerBtn->onClickFunc = [toggleLayerText, mapDrawer](int z_index, bool active) {
			mapDrawer->toggle(z_index);
		};
		

	}

	//mapDrawer = mapLayers[0];

	auto tileSheet = pallet.display->loadTexture("../Assets/Sprites/HaGameEngine/Environment/mapbuilder-tiles.png");
	auto tiles = SpriteSheetLoader::load("mapbuilder-tiles.txt");

	const int tileSize = 50;

	for (int i = 0; i < tiles.size(); i++) {
		auto palletTile = pallet.add();
		palletTile->addComponent(new SpriteRenderer(tileSheet, tiles[i], tileSize, tileSize));
		palletTile->move(Vector(i * tileSize, 0));
		auto palletTileBtn = palletTile->addComponent(new TileButton(tileSize, tileSize));
		palletTileBtn->spriteSheet = tileSheet;
		palletTileBtn->cell = tiles[i];
		palletTileBtn->onClickFunc = [mapDrawer](PalletItem * pallet) {
			mapDrawer->setPallet(pallet->spriteSheet, pallet->spriteSheetCell);
		};
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
