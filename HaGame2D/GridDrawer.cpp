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
			if (brushSize > 1) {
				for (int i = brushSize / -2; i < brushSize / 2; i++) {
					for (int j = brushSize / -2; j < brushSize / 2; j++) {
						int x = mIndex.x + i;
						int y = mIndex.y + j;
						if (x >= 0 && x < grid.cols && y >= 0 && y < grid.rows) {
							auto palletCell = grid.get(mIndex.x + i, mIndex.y + j);
							auto pallet = palletCell.layers[z_index];
							pallet->type = currentPallet->type;
							pallet->color = currentPallet->color;
							pallet->spriteSheet = currentPallet->spriteSheet;
							pallet->spriteSheetCell = currentPallet->spriteSheetCell;
						}
					}
				}
			}
			else {
				auto palletCell = grid.get(mIndex.x, mIndex.y);
				auto pallet = palletCell.layers[z_index];
				pallet->type = currentPallet->type;
				pallet->color = currentPallet->color;
				pallet->spriteSheet = currentPallet->spriteSheet;
				pallet->spriteSheetCell = currentPallet->spriteSheetCell;
			}
		}

		else if (input->fire2 && display->inDisplayPort(Vector(input->mouseX, input->mouseY)) && mIndex.x < grid.cols && mIndex.y < grid.rows && mIndex.x >= 0 && mIndex.y >= 0) {
			if (brushSize > 1) {
				for (int i = brushSize / -2; i < brushSize / 2; i++) {
					for (int j = brushSize / -2; j < brushSize / 2; j++) {
						int x = mIndex.x + i;
						int y = mIndex.y + j;
						if (x >= 0 && x < grid.cols && y >= 0 && y < grid.rows) {
							auto palletCell = grid.get(mIndex.x + i, mIndex.y + j);
							auto pallet = palletCell.layers[z_index];
							pallet->type = PalletType::Empty;
						}
					}
				}
			}
			else {
				auto palletCell = grid.get(mIndex.x, mIndex.y);
				auto pallet = palletCell.layers[z_index];
				pallet->type = PalletType::Empty;
			}
		}


		//draw a little box to show where you'll draw at.
		Vector scale = Vector(relativeWidth / width, relativeHeight / height);
		float relColSize = scale.x * grid.colSize;
		float relRowSize = scale.y * grid.rowSize;
		Vector pos = Vector(transform->relativePosition.x + (mIndex.x - brushSize / 2) * relColSize, transform->relativePosition.y + (mIndex.y - brushSize / 2) * relRowSize);
		Vector size = Vector(brushSize * grid.colSize, brushSize * grid.rowSize);
		
		size.x = size.x * scale.x;
		size.y = size.y * scale.y;

		display->drawRect(pos.x + 1, pos.y + 1, size.x, size.y, Color::red(), 18);

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
