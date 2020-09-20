#pragma once
#include "Grid.h"
#include "Set.h"
#include "Color.h"
#include "BoxComponent.h"
#include "GameObject.h"

struct BinaryGridNode {
	int x;
	int y;
	RGB fillColor;
	std::string tag;
};

class BinaryGrid : public BoxComponent
{
	double _height, _width;
	int _xPartitions, _yPartitions;
	double _nodeWidth, _nodeHeight;

	Grid<bool> _grid;

	Set<BinaryGridNode> _activeNodes = Set<BinaryGridNode>([this](BinaryGridNode node) {
		return node.y * _xPartitions + node.x;
	});

	RGB _gridColor;

	int hash(int x, int y) {
		return x * _xPartitions + y;
	}

public:

	BinaryGrid(double width, double height, int xPartitions, int yPartitions, RGB gridColor = Color::blue()) : BoxComponent(width, height) {
		_width = width;
		_height = height;
		_xPartitions = xPartitions;
		_yPartitions = yPartitions;
		_gridColor = gridColor;

		_grid = Grid<bool>(_width, _height, _yPartitions, _xPartitions);
	}

	void activate(int x, int y, RGB fillColor, std::string tag) {
		_activeNodes.add(BinaryGridNode{ x, y, fillColor, tag });
	}

	void deactivate(int x, int y) {
		_activeNodes.remove(BinaryGridNode{ x, y });
	}

	void update() {
		double xSize = relativeWidth / _grid.cols;
		double ySize = relativeHeight / _grid.rows;

		for (int i = 0; i <= _grid.cols; i++) {
			display->drawLine(transform->relativePosition.x + i * xSize, transform->relativePosition.y, transform->relativePosition.x + i * xSize, transform->relativePosition.y + relativeHeight, _gridColor, 17);
		}

		for (int i = 0; i <= _grid.rows; i++) {
			display->drawLine(transform->relativePosition.x, transform->relativePosition.y + i * ySize, transform->relativePosition.x + relativeWidth, transform->relativePosition.y + i * ySize, _gridColor, 18);
		}

		double nodeWidth = relativeWidth / _xPartitions;
		double nodeHeight = relativeHeight / _yPartitions;

		for (BinaryGridNode node : _activeNodes.set()) {
			display->fillRect(transform->relativePosition.x + node.x * nodeWidth + 1, transform->relativePosition.y + node.y * nodeHeight + 1, nodeWidth, nodeHeight, node.fillColor);
		}
	}

	int rows() {
		return _xPartitions;
	}

	int cols() {
		return _yPartitions;
	}

	void iterateNodesWhere(std::string tag, std::function<void(BinaryGridNode)> iterFunc) {
		_activeNodes.iterate([tag, iterFunc](BinaryGridNode node) {
			if (node.tag == tag) {
				iterFunc(node);
			}
		});
	}
};

