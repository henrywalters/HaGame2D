#include "stdafx.h"
#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	uid = Random::number(1000000, 9999999);
	tags = std::vector<std::string>();
	position = Vector::Zero();
	origin = Vector::Zero();
	scale = Vector::Identity();
	childGameObjects = {};
	rotation = 0;
}


GameObject::~GameObject()
{
	destroyComponents();
	for (auto gameObject : childGameObjects) {
		delete gameObject;
	}
}

GameObject* GameObject::setName(char * _name)
{
	name = _name;
	return this;
}

GameObject * GameObject::setPosition(Vector pos) {
	position = pos;
	return this;
}

GameObject * GameObject::setRotation(float theta) {
	rotation = theta;
	return this;
}

GameObject * GameObject::setScale(Vector newScale) {
	scale = newScale;
	return this;
}

GameObject * GameObject::move(Vector delta) {
	position += delta;
	return this;
}

GameObject * GameObject::rotate(float theta) {
	rotation += theta;
	return this;
}

GameObject* GameObject::setOrigin(Vector newOrigin) {
	origin = newOrigin;
	return this;
}


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
	GameObject * child = new GameObject();
	return add(child);
}

GameObject* GameObject::add(GameObject* child)
{
	child->parentGameObject = this;
	childGameObjectCount += 1;
	childGameObjects.push_back(child);
	return child;
}

void GameObject::initialize(Display * display, Input * input, Scene * scene)
{
	for (auto gameObject : childGameObjects) {
		gameObject->initialize(display, input, scene);
	}

	for (int i = 0; i < componentCount; i++) {
		components[i]->initialize(display, input, scene);
		components[i]->onCreate();
	}
}
