#pragma once
#include "GameObject.h"
#include "BoxComponent.h"
#include "Grid.h"

class GridLines : public BoxComponent
{
public:

	Grid<bool> grid;

	RGB color = Color::blue();

	GridLines(float width, float height, int xpartitions, int ypartitions);
	~GridLines();

	void update();
};

