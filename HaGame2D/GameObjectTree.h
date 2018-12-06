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

};
