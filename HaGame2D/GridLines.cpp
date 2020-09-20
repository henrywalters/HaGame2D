#include "stdafx.h"
#include "GridLines.h"


GridLines::GridLines(double _width, double _height, int x, int y)
{
	grid = Grid<bool>(_width, _height, y, x);
	height = _height;
	width = _width;
}


GridLines::~GridLines()
{
}

void GridLines::update() {

	double xSize = relativeWidth / grid.cols;
	double ySize = relativeHeight / grid.rows;

	for (int i = 0; i <= grid.cols; i++) {
		display->drawLine(transform->relativePosition.x + i * xSize, transform->relativePosition.y, transform->relativePosition.x + i * xSize, transform->relativePosition.y + relativeHeight, color, 17);
	}

	for (int i = 0; i <= grid.rows; i++) {
		display->drawLine(transform->relativePosition.x, transform->relativePosition.y + i * ySize, transform->relativePosition.x + relativeWidth, transform->relativePosition.y + i * ySize, color, 18);
	}
}