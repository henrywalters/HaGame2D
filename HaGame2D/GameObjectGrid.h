#pragma once
#include "HaGame2D.h"

struct int2 {
	int x, y;
};

inline bool operator< (const int2& lhs, const int2& rhs) {
	return std::make_pair(lhs.x, lhs.y) < std::make_pair(rhs.x, rhs.y);
}

class GameObjectGrid {
public:
	std::map<int2, std::vector<GameObject*>> grid;
	Vector origin;
	Vector cellSize;

	GameObjectGrid(Vector _origin, Vector _cellSize) {
		grid = std::map<int2, std::vector<GameObject*>>();
		origin = _origin;
		cellSize = _cellSize;
	}

	void clear() {
		grid.clear();
	}

	int2 getIndex(Vector pos) {
		pos -= origin;
		return int2{
			(int) floor(pos.x / cellSize.x),
			(int) floor(pos.y / cellSize.y)
		};
	}

	bool hasObjects(Vector pos) {
		return grid.find(getIndex(pos)) != grid.end();
	}

	void add(GameObject* go) {
		if (!hasObjects(go->position)) {
			grid[getIndex(go->position)] = {};
		}
		grid[getIndex(go->position)].push_back(go);
	}

	void addMany(std::vector<GameObject*> gos) {
		for (auto go : gos) {
			add(go);
		}
	}

	std::vector<GameObject*> getObjects(Vector pos) {
		return hasObjects(pos) ? grid[getIndex(pos)] : std::vector<GameObject*>();
	}

	std::vector<GameObject*> getNeighbors(GameObject* go) {
		return getObjects(go->position);
	}
};