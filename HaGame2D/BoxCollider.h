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

	const float epsilon = 3;

public:

	float width;
	float height;

	BoxCollider(float width, float height);
	BoxCollider(Vector size);
	~BoxCollider();

	Box getBox() {
		Box box = { transform->position.x, transform->position.y, width, height };
		return box;
	}
};

