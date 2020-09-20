#pragma once
#include "Component.h"
#include "Vector.h"
#include <queue>
#include <vector>
#include <type_traits>
#include "Display.h"
#include <map>
#include <string>
#include "BoxComponent.h"
#include "Random.h"

class Scene;

const int MAX_COMPONENTS = 100;
const int MAX_TAGS = 10;

const std::string IGNORE_ORIGIN = "IGNORE_ORIGIN";

class GameObject
{

private:

	Vector oldPosition;
	Vector oldScale;
	double oldRotation;

	Vector oldOrigin;

	int tagCount = 0;
	std::vector<std::string> tags;

public:

	bool positionUpdated = false;
	bool scaleUpdated = false;
	bool rotationUpdated = false;

	Vector positionDelta;
	Vector scaleDelta;
	double rotationDelta;


	Vector position;
	Vector relativePosition;

	Vector scale;
	Vector relativeScale;

	double rotation;
	Vector origin;

	bool staticObject = false;
	bool trackedOffScreen = true;
	
	char * name = NULL;
	char * tag = NULL;
	char * id = NULL;

	long uid;

	int componentCount = 0;

	int z_index = Z_DEFAULT;

	Component * components[MAX_COMPONENTS];

	std::vector<GameObject *> childGameObjects;
	int childGameObjectCount = 0;
	GameObject * parentGameObject = NULL;
	
	bool active = true;

	GameObject(); // : GameObjectTree(this) {};

	virtual ~GameObject();

	GameObject* setName(char * _name);
	
	//Directly set the three primary properties
	GameObject * setPosition(Vector position);
	GameObject * setRotation(double rotation);
	GameObject * setScale(Vector scale);

	//Relatively alter the three primary properties
	GameObject * move(Vector positionDelta);
	GameObject * rotate(double rotationDelta);

	//origin methods - controls the center of a game object
	GameObject * setOrigin(Vector origin);

	virtual void draw(Display *display){};
	void update();

	void addTag(std::string tag) {
		tags.push_back(tag);
		tagCount += 1;
	}

	GameObject* addTagAnd(std::string tag) {
		addTag(tag);
		return this;
	}

	bool hasTag(std::string tag) {
		for (int i = 0; i < tags.size(); i++) {
			if (tags[i] == tag) {
				return true;
			}
		}
		return false;
	}

	void removeTag(std::string tag) {
		for (int i = 0; i < tags.size(); i++) {
			if (tags[i] == tag) {
				tags.erase(tags.begin() + i);
				tagCount--;
				return;
			}
		}
	}

	bool isWithinViewport(Matrix viewport);

	//Component Methods
	template<class T>
	T * addComponent(T * component);

	template<class T>
	GameObject* addComponents(std::vector<T> components);

	template<class T>
	GameObject* addComponentAnd(T* component);

	template<class T>
	GameObject* addComponentAnd(T* component, std::function<void(T *)> callback);

	template<class T>
	T * getComponent();

	template<class T>
	T * getComponentInChildren();

	template<class T>
	T * getComponentInParent();

	//GameObject Methods
	GameObject * add();

	GameObject* add(GameObject* child);

	void destroyComponents() {
		for (int i = 0; i < componentCount; i++) {
			components[i]->onDestroy();
			components[i]->active = false;
			//free(components[i]);
		}
		componentCount = 0;
	}

	void initialize(Display *display, Input *input, Scene * scene);

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
inline GameObject* GameObject::addComponents(std::vector<T> components) {
	for (component : components) {
		addComponent(component);
	}
	return this;
}

template<class T>
inline GameObject* GameObject::addComponentAnd(T* component)
{
	if (std::is_base_of<Component, T>::value) {
		if (componentCount < MAX_COMPONENTS - 1) {
			components[componentCount] = component;
			components[componentCount]->transform = this;
			componentCount += 1;
		}
	}

	return this;
}

template<class T>
inline GameObject* GameObject::addComponentAnd(T* component, std::function<void(T*)> callback)
{
	if (std::is_base_of<Component, T>::value) {
		if (componentCount < MAX_COMPONENTS - 1) {
			components[componentCount] = component;
			components[componentCount]->transform = this;
			componentCount += 1;

			callback(component);
		}
	}

	return this;
}

template<class T>
inline T * GameObject::getComponent() 
{
	for (int i = 0; i < componentCount; i++) {
		
		if (components[i]->isInstanceOf<T>()) {
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