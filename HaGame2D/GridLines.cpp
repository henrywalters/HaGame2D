#include "stdafx.h"
#include "GridLines.h"


GridLines::GridLines(float _width, float _height, int x, int y)
{
	grid = Grid<int>(_width, _height, y, x);
	height = _height;
	width = _width;
}


GridLines::~GridLines()
{
}

void GridLines::update() {

	float xSize = relativeWidth / grid.cols;
	float ySize = relativeHeight / grid.rows;

	for (int i = 0; i <= grid.cols; i++) {
		display->drawLine(transform->relativePosition.x + i * xSize, transform->relativePosition.y, transform->relativePosition.x + i * xSize, transform->relativePosition.y + relativeHeight, color, 17);
	}

	for (int i = 0; i <= grid.rows; i++) {
		display->drawLine(transform->relativePosition.x, transform->relativePosition.y + i * ySize, transform->relativePosition.x + relativeWidth, transform->relativePosition.y + i * ySize, color, 18);
	}


	/*for (int i = 0; i < xPartitions; i++) {
		for (int j = 0; j < yPartitions; j++) {

			int mX = input->mouseXGlobal;
			int mY = input->mouseYGlobal;
			if (mX >= transform->position.x + i * x && mX < transform->position.x + (i + 1) * x && mY >= transform->position.y + j * y && mY < transform->position.y + (j + 1) * y) {
				display->fillRect(transform->relativePosition.x + i * xSize, transform->relativePosition.y + j * ySize, xSize, ySize, Color::white(), 18);
			}
			else {
				display->drawRect(transform->relativePosition.x + i *xSize, transform->relativePosition.y + j * ySize, xSize, ySize, Color::white(), 18);
			}

			
		}
	}

	*/
}