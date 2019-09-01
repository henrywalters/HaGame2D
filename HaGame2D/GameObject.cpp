#include "stdafx.h"
#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	position = Vector::Zero();
	origin = Vector::Zero();
	scale = Vector::Identity();
	rotation = 0;

	for (int i = 0; i < MAX_TAGS; i++) {
		tags[i] = "";
	}
}


GameObject::~GameObject()
{
	for (int i = 0; i < componentCount; i++) {
		delete components[i];		
	}
	componentCount = 0;
	if (childGameObjectCount == 0) {
		active = false;
	}
	else {
		for (auto gameObject : childGameObjects) {
			delete gameObject;
		}
	}
}

GameObject* GameObject::setZIndex(int zIndex)
{
	z_index = zIndex;
	return this;
}

GameObject* GameObject::setPosition(Vector pos) {
	position = pos;
	return this;
}

GameObject* GameObject::setRotation(float theta) {
	rotation = theta;
	return this;
}

GameObject* GameObject::setScale(Vector newScale) {
	scale = newScale;
	return this;
}

// Move the game object relative to its current position and rotation;
GameObject * GameObject::move(Vector delta) {
	delta.x = delta.x * cos(rotation) - delta.y * sin(rotation);
	delta.y = delta.x * sin(rotation) + delta.y * cos(rotation);
	std::cout<< rotation;
	std::cout << delta.toString();
	position += delta;
	return this;
}

GameObject* GameObject::rotate(float theta) {
	rotation += theta;
	return this;
}

/*
void GameObject::moveOrigin(Vector newOrigin) {
	origin += newOrigin;
	newOrigin.display();
	origin.display();
	relativePosition = position - origin;
}

void GameObject::setOrigin(Vector newOrigin) {
	origin = newOrigin;
	relativePosition = position - origin;
}
*/

void GameObject::update() {

	positionUpdated = oldPosition != position ? true : false;
	scaleUpdated = oldScale != scale ? true : false;
	rotationUpdated = oldRotation != rotation ? true : false;

	positionDelta = position - oldPosition;
	scaleDelta = scale - oldScale;
	rotationDelta = rotation - oldRotation;

	oldPosition = position;
	oldRotation = rotation;
	oldScale = scale;

	for (int i = 0; i < componentCount; i++) {
		if (components[i]->active) {
			components[i]->update();
		}
	}
}

bool GameObject::isWithinViewport(Matrix viewport)
{
	BoxComponent * box = getComponent<BoxComponent>();
	float bx, by, bw, bh;

	bx = position.x;
	by = position.y;

	if (box == NULL) {
		bw = 1; bh = 1;
	}
	else {
		bw = box->width;
		bh = box->height;
	}
	
	bool isWithin = false;

	if (Vector(bx, by).isContainedBy(viewport.get(0), viewport.get(1), viewport.get(2), viewport.get(3))) {
		isWithin = true;
	}
	else if (Vector(bx + bw, by).isContainedBy(viewport.get(0), viewport.get(1), viewport.get(2), viewport.get(3))) {
		isWithin = true;
	}
	else if (Vector(bx, by + bh).isContainedBy(viewport.get(0), viewport.get(1), viewport.get(2), viewport.get(3))) {
		isWithin = true;
	}
	else if (Vector(bx + bw, by + bh).isContainedBy(viewport.get(0), viewport.get(1), viewport.get(2), viewport.get(3))) {
		isWithin = true;
	}

	return isWithin;
}

GameObject * GameObject::add() {
	GameObject * gameObject = new GameObject();
	gameObject->parentGameObject = this;
	childGameObjectCount += 1;
	childGameObjects.push_back(gameObject);
	return gameObject;
}