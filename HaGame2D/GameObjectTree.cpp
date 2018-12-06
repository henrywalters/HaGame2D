#include "stdafx.h"
#include "GameObjectTree.h"

GameObjectTree::GameObjectTree()
{
	//Create an empty gameobject to be the parent
	root = new GameObject();
}

GameObjectTree::GameObjectTree(GameObject * parent) {
	root = parent;
}


GameObjectTree::~GameObjectTree()
{
	free(root);
}

GameObject * GameObjectTree::add() {
	GameObject * gameObject = new GameObject();
	root->childGameObjects.push_back(gameObject);
	root->childGameObjectCount += 1;
	gameObject->parentGameObject = root;
	return gameObject;
}

GameObject * GameObjectTree::add(GameObject * gameObject) {
	root->childGameObjects.push_back(gameObject);
	root->childGameObjectCount += 1;
	gameObject->parentGameObject = root;
	return gameObject;
}


std::vector<GameObject *> GameObjectTree::getGameObjects() {
	std::vector<GameObject *> objects;
	std::queue<GameObject *> queue;

	queue.push(root);

	while (!queue.empty()) {
		GameObject * current = queue.front();

		objects.push_back(current);
		queue.pop();

		for (int i = 0; i < current->childGameObjectCount; i++) {
			queue.push(current->childGameObjects[i]);
		}
	}

	return objects;
}