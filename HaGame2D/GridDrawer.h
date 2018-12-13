#pragma once
#include "BoxComponent.h"
#include "Component.h"
#include "GameObject.h"
#include "Grid.h"
#include "SpriteSheetLoader.h"
#include "MapLoader.h"
#include <vector>
const int Z_INDICES = 5;

enum PalletType {
	Empty,
	ColorPallet,
	SpritePallet
};

struct PalletItem {
	PalletType type;
	RGB color;
	Texture spriteSheet;
	SpriteSheetCell spriteSheetCell;
};

struct GridCell {
	PalletItem * layers[Z_INDICES];
};

class GridDrawer : public BoxComponent
{
public:

	int z_index = 0;

	bool activeLayers[Z_INDICES];

	Grid<GridCell> grid;

	PalletItem *currentPallet = new PalletItem();

	int brushSize = 4;

	GridDrawer(float _width, float _height, int rows, int cols);
	~GridDrawer();

	void update();

	void setPallet(Texture spriteSheet, SpriteSheetCell cell) {
		currentPallet->type = PalletType::SpritePallet;
		currentPallet->spriteSheetCell = cell;
		currentPallet->spriteSheet = spriteSheet;
	}

	void setPallet(RGB color) {
		currentPallet->type = PalletType::ColorPallet;
		currentPallet->color = color;
	}

	void setLayer(int layer) {
		if (layer >= 0 && layer < Z_INDICES) {
			z_index = layer;
		}
	}

	void setBrushSize(int size) {
		if (size >= 1 && (size == 1 || size % 2 == 0)) {
			brushSize = size;
		}
		else {
			std::cout << "WARNING - attempted to set brush size to odd number. This is not allowed!";
		}
	}

	void clearPallet() {
		currentPallet->type = PalletType::Empty;
	}

	void activate(int layer) {
		activeLayers[layer] = true;
	}

	void deactivate(int layer) {
		activeLayers[layer] = false;
	}

	void toggle(int layer) {
		activeLayers[layer] = !activeLayers[layer];
	}

	void save(char * filePath) {
		std::vector<MapCell> cells;

		for (int i = 0; i < grid.rows; i++) {
			for (int j = 0; j < grid.cols; j++) {
				GridCell gridCell = grid.get(i, j);
				for (int k = 0; k < Z_INDICES; k++) {
					PalletItem *pallet = gridCell.layers[k];
					if (pallet->type == PalletType::SpritePallet) {
						MapCell cell;

						cell.x = j;
						cell.y = i;
						cell.width = 1;
						cell.height = 1;
						cell.z_index = k;
						cell.key = pallet->spriteSheetCell.cellType;
						cells.push_back(cell);
					}
				}
			}
		}

		MapLoader::save(filePath, cells);
	}

	void tryLoad(char *filePath, Texture spriteSheet) {
		std::vector<MapCell> cells = MapLoader::load(filePath);
		if (cells.size() > 0) {
			std::cout << "Clearing all tiles \n";
			clearAll();
			for (MapCell cell : cells) {
				GridCell gridCell = grid.get(cell.y, cell.x);
				auto mapTileDef = SpriteSheetLoader::getSpriteMap("mapbuilder-tiles.txt");

				gridCell.layers[cell.z_index]->type = PalletType::SpritePallet;
				gridCell.layers[cell.z_index]->spriteSheet = spriteSheet;
				gridCell.layers[cell.z_index]->spriteSheetCell = mapTileDef[cell.key];
				
			}
		}
	}

	void clearAll() {
		GridCell cell;
		for (int i = 0; i < grid.rows; i++) {
			for (int j = 0; j < grid.cols; j++) {
				for (int k = 0; k < Z_INDICES; k++) {
					PalletItem *item = new PalletItem();
					item->type = PalletType::Empty;
					cell.layers[k] = item;
				}
				grid.set(i, j, cell);
			}
		}
	}

	void onCreate() {

		GridCell cell;

		for (int k = 0; k < Z_INDICES; k++) {
			PalletItem *item = new PalletItem();
			item->type = PalletType::Empty;
			cell.layers[k] = item;

			activeLayers[k] = true;
		}

		grid.initialize(cell);
		
		currentPallet->type = PalletType::ColorPallet;
		currentPallet->color = Color::white();

		clearAll();
	}
};

