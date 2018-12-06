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
		free(components[i]);		
	}
	componentCount = 0;
}

void GameObject::setPosition(Vector pos) {
	position = pos;
}

void GameObject::setRotation(float theta) {
	rotation = theta;
}

void GameObject::setScale(Vector newScale) {
	scale = newScale;
}

void GameObject::move(Vector delta) {
	position += delta;
}

void GameObject::rotate(float theta) {
	rotation += theta;
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
		components[i]->update();
	}
}

GameObject * GameObject::add() {
	GameObject * gameObject = new GameObject();
	gameObject->parentGameObject = this;
	childGameObjectCount += 1;
	childGameObjects.push_back(gameObject);
	return gameObject;
}