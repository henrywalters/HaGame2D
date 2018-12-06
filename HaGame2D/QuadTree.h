#pragma once
#include "GameObject.h"
#include "Component.h"
#include "BoxCollider.h"
#include <vector>


struct Quadrant {
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;

	Quadrant *parent;
	Quadrant *quad1;
	Quadrant *quad2;
	Quadrant *quad3;
	Quadrant *quad4;

	std::vector<GameObject *> gameObjects;

	void init(Quadrant * quadrant, Box box) { 
		x = box.x;
		y = box.y; 
		width = box.width;
		height = box.height;
		parent = quadrant;
		quad1 = NULL;
		quad2 = NULL;
		quad3 = NULL;
		quad4 = NULL;
		gameObjects = std::vector<GameObject *>();
	}

	Box getBox() {
		return Box{ x, y, width, height };
	}

	bool isSplit() {
		return quad1 != NULL;
	}

	bool contains(Vector point) {
		return (point.x >= x && point.x < x + width && point.y >= y && point.y < y + height);
	}

	bool operator == (const Quadrant &other) {
		return (x == other.x && y == other.y && width == other.width && height == other.height);
	}

	bool operator -= (const Quadrant &other) {
		return !(*this == other);
	}

	std::vector<Quadrant *> getPartiallyBoundingQuads(GameObject * gameObject) {
		std::vector<Quadrant *> boundingBoxes;
		BoxCollider * object = gameObject->getComponent<BoxCollider>();

		if (object != NULL) {
			Box box = object->getBox();
			if (quad1->getBox().collidingWith(box)) {
				boundingBoxes.push_back(quad1);
			}
			if (quad2->getBox().collidingWith(box)) {
				boundingBoxes.push_back(quad2);
			}
			if (quad3->getBox().collidingWith(box)) {
				boundingBoxes.push_back(quad3);
			}
			if (quad4->getBox().collidingWith(box)) {
				boundingBoxes.push_back(quad4);
			}
		}

		return boundingBoxes;
	}

	Quadrant * getBoundingQuad(GameObject * gameObject) {
		
		BoxCollider * object = gameObject->getComponent<BoxCollider>();

		if (object != NULL && quad1 != NULL) {
			Box other = object->getBox();

			if (other.boundedBy(quad1->getBox())) {
				return quad1;
			}
			if (other.boundedBy(quad2->getBox())) {
				return quad2;
			}
			if (other.boundedBy(quad3->getBox())) {
				return quad3;
			}
			if (other.boundedBy(quad4->getBox())) {
				return quad4;
			}
		}

		return NULL;
	}

	Quadrant * getBoundingQuad(Vector point) {

		if (!isSplit()) {
			return this;
		}

		if (quad1->contains(point)) {
			return quad1;
		}
		else if (quad2->contains(point)) {
			return quad2;
		}
		else if (quad3->contains(point)) {
			return quad3;
		}
		else if (quad4->contains(point)) {
			return quad4;
		}
		else {
			//std::cout << "WARNING - Failed to find child that contains point: ";
			//point.display();
			return this;
		}
	}

};

struct QuadSet {
	Quadrant * quads[4];
	int quadCount = 0;

	void add(Quadrant * quad) {
		if (quadCount < 4) {
			quads[quadCount] = quad;
			quadCount += 1;
		}
	}
};

class QuadTree
{
private:
	std::vector<Quadrant *> quadrants;
	bool showQuadrants = false;
public:

	Quadrant * root;

	Display * display;

	const int MAX_CHILDREN_IN_QUADRANT = 8;
	const int MAX_DEPTH = 10;

	QuadTree(float width, float height);
	~QuadTree();

	void clear();
	void split(Quadrant * quadrant);
	void insert(GameObject * gameObject);
	QuadSet find(GameObject * gameObject);
	std::vector<GameObject *> getNeighbors(GameObject * gameObject);

	void setDisplay(Display * _display) {
		display = _display;
		showQuadrants = true;
	}

};

