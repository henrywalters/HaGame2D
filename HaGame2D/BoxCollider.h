#pragma once
#include "GameObject.h"
#include "CollisionComponent.h"
#include <vector>
#include <string>
#include "Math.h"
#include "Circle.h"
#include "Box.h";

class BoxCollider : public CollisionComponent
{

	const double epsilon = 3;

public:

	double width;
	double height;

	BoxCollider(double width, double height);
	BoxCollider(Vector size);
	~BoxCollider();

	Box getBox() {
		Box box = { transform->position.x, transform->position.y, width, height };
		return box;
	}
};

