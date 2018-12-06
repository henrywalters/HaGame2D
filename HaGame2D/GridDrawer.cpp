#include "stdafx.h"
#include "GridDrawer.h"


GridDrawer::GridDrawer(float _width, float _height, int rows, int cols)
{
	grid = Grid<GridCell>(_width, _height, rows, cols);
	width = _width; height = _height;
}


GridDrawer::~GridDrawer()
{
}

void GridDrawer::update() {
	if (active) {
		float mX = input->mouseXGlobal;
		float mY = input->mouseYGlobal;

		GridIndex mIndex = grid.getSpatialIndex(transform->position, Vector(mX, mY));

		if (input->fire1 && display->inDisplayPort(Vector(input->mouseX, input->mouseY)) && mIndex.x < grid.cols && mIndex.y < grid.rows && mIndex.x >= 0 && mIndex.y >= 0) {
			auto palletCell = grid.get(mIndex.x, mIndex.y);
			auto pallet = palletCell.layers[z_index];
			pallet->type = currentPallet->type;
			pallet->color = currentPallet->color;
			pallet->spriteSheet = currentPallet->spriteSheet;
			pallet->spriteSheetCell = currentPallet->spriteSheetCell;
		}

		if (input->fire2 && display->inDisplayPort(Vector(input->mouseX, input->mouseY)) && mIndex.x < grid.cols && mIndex.y < grid.rows && mIndex.x >= 0 && mIndex.y >= 0) {
			auto palletCell = grid.get(mIndex.x, mIndex.y);
			auto pallet = palletCell.layers[z_index];
			pallet->type = PalletType::Empty;
		}

		GridIndex iter = grid.begin();

		for (int i = 0; i < grid.rows; i++) {
			for (int j = 0; j < grid.cols; j++) {

				GridCell cell = grid.get(i, j);

				for (int k = 0; k < Z_INDICES; k++) {
					if (activeLayers[k]) {
						auto pallet = cell.layers[k];
						switch (pallet->type) {
						case PalletType::Empty:
							//Do nothing 
							break;
						case PalletType::ColorPallet:
							display->fillRect(transform->relativePosition.x + i * (relativeWidth / grid.cols) - 1, transform->relativePosition.y + j * (relativeHeight / grid.rows) - 1, relativeWidth / grid.cols, relativeHeight / grid.rows, pallet->color, k);
							break;
						case PalletType::SpritePallet:
							auto clip = TextureRect{ pallet->spriteSheetCell.x, pallet->spriteSheetCell.y, pallet->spriteSheetCell.width, pallet->spriteSheetCell.height };
							auto rect = TextureRect{ transform->relativePosition.x + i * (relativeWidth / grid.cols), transform->relativePosition.y + j * (relativeHeight / grid.rows), relativeWidth / grid.cols, relativeHeight / grid.rows };
							display->drawTexture(rect, pallet->spriteSheet, clip, k);
							break;
						}
					}
				}

				iter = grid.next(iter);
			}
		}
	}
}
