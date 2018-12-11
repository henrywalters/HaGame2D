#pragma once
#include "Component.h"
#include "Vector.h"
#include <queue>
#include <vector>
#include <type_traits>
#include "Display.h"
#include <map>
#include <string>

class Scene;

const int MAX_COMPONENTS = 100;
const int MAX_TAGS = 10;

const std::string IGNORE_ORIGIN = "IGNORE_ORIGIN";

class GameObject
{

private:

	Vector oldPosition;
	Vector oldScale;
	float oldRotation;

	Vector oldOrigin;

	int tagCount = 0;
	std::string tags[MAX_TAGS];

public:

	bool positionUpdated = false;
	bool scaleUpdated = false;
	bool rotationUpdated = false;

	Vector positionDelta;
	Vector scaleDelta;
	float rotationDelta;


	Vector position;
	Vector relativePosition;

	Vector scale;
	Vector relativeScale;

	float rotation;
	Vector origin;

	bool staticObject = false;
	
	char * name = NULL;
	char * tag = NULL;
	char * id = NULL;

	int componentCount = 0;

	int z_index = Z_DEFAULT;

	Component * components[MAX_COMPONENTS];

	std::vector<GameObject *> childGameObjects;
	int childGameObjectCount = 0;
	GameObject * parentGameObject = NULL;
	
	bool active = true;

	GameObject(); // : GameObjectTree(this) {};

	virtual ~GameObject();

	//Directly set the three primary properties
	void setPosition(Vector position);
	void setRotation(float rotation);
	void setScale(Vector scale);

	//Relatively alter the three primary properties
	void move(Vector positionDelta);
	void rotate(float rotationDelta);

	//origin methods - Remove origin handling at all from the object. Only care about world position;
	//void setOrigin(Vector origin);
	//void moveOrigin(Vector origin);

	virtual void draw(Display *display){};
	void update();

	void addTag(std::string tag) {
		if (tagCount < MAX_TAGS - 1) {
			tags[tagCount] = tag;
			tagCount += 1;
		}
		else {
			std::cout << "WARNING - Max tags for game object exceeded\n";
		}
	}

	bool hasTag(std::string tag) {
		for (int i = 0; i < tagCount; i++) {
			if (tags[i] == tag) {
				return true;
			}
		}
		return false;
	}

	//Component Methods
	template<class T>
	T * addComponent(T * component);

	template<class T>
	T * getComponent();

	template<class T>
	T * getComponentInChildren();

	template<class T>
	T * getComponentInParent();

	//GameObject Methods
	GameObject * add();


};

template<class T>
inline T * GameObject::addComponent(T * component)
{
	if (std::is_base_of<Component, T>::value) {
		if (componentCount < MAX_COMPONENTS - 1) {
			components[componentCount] = component;
			components[componentCount]->transform = this;
			componentCount += 1;
		}
	}

	return component;
}

template<class T>
inline T * GameObject::getComponent() 
{
	for (int i = 0; i < componentCount; i++) {
		
		if ((*components[i]).isInstanceOf<T>()) {
			return dynamic_cast<T *>(components[i]);
		}
	}

	return NULL;
}

template <class T>
inline T * GameObject::getComponentInChildren() {
	for (int i = 0; i < childGameObjectCount; i++) {
		for (int j = 0; j < childGameObjects[i]->componentCount; j++) {
			if (typeid(T).name() == typeid(*childGameObjects[i]->components[j]).name()) {
				return (T *)childGameObjects[i]->components[j];
			}
		}
	}
}

template <class T>
inline T * GameObject::getComponentInParent() {
	for (int i = 0; i < parentGameObject->componentCount; i++) {
		//if (typeid(T).name() == typeid(*parentGameObject->components[i]).name()) {
		//	return (T *)parentGameObject->components[i];
		//}

		if ((*parentGameObject->components[i]).isInstanceOf<T>()) {
			return dynamic_cast<T *>(parentGameObject->components[i]);
		}
	}
}