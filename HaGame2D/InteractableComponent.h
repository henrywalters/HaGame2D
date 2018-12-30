#pragma once
#include "Component.h"
#include "Inventory.h"
#include "BoxComponent.h"
#include "BoxCollider.h"
#include "InteractionRequirement.h"

class InteractableComponent : public BoxComponent
{
	BoxCollider * collider = NULL;
	bool hasColliderSet = false;

	bool canUse = true;

	std::string message = "";

	std::vector<InteractionRequirement> requirements;

public:
	InteractableComponent(float width, float height);
	~InteractableComponent();
	
	void setMessage(std::string _message) {
		message = _message;
	}

	void onCreate();
	void update();

	void addRequirement(InteractionRequirement requirement);

	virtual void use() {};
	virtual void useItemOn(InventoryItem item) {};
};

