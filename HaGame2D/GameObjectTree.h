#pragma once
#include "GameObject.h"
#include <vector>
#include <queue>

class GameObjectTree
{
public:
	GameObject * root;

	GameObjectTree();
	GameObjectTree(GameObject * parent);
	~GameObjectTree();

	//Adds an empty game object to the game object tree.
	GameObject * add();

	//Adds a pre-existing game object to the game object tree.
	GameObject * add(GameObject * gameObject);

	std::vector<GameObject *> getGameObjects();
	
	template<class T>
	std::vector<GameObject*> getGameObjectsWhere();
};

template<class T>
inline std::vector<GameObject*> GameObjectTree::getGameObjectsWhere() {
	std::vector<GameObject*> gameObjects;
	for (auto gameObject : getGameObjects()) {
		if (gameObject->getComponent<T>() != NULL) {
			gameObjects.push_back(gameObject);
		}
	}
	return gameObjects;
}
