#include "stdafx.h"
#include "InteractableComponent.h"

InteractableComponent::InteractableComponent(float _width, float _height)
{
	width = _width;
	height = _height;
}


InteractableComponent::~InteractableComponent()
{
}

void InteractableComponent::onCreate() {
	collider = transform->getComponent<BoxCollider>();
	if (collider != NULL) {
		hasColliderSet = true;
	}
}

void InteractableComponent::addRequirement(InteractionRequirement *requirement) {
	requirements.push_back(requirement);
}

void InteractableComponent::clearRequirements()
{
	requirements.clear();
}

void InteractableComponent::update() {
	bool hasUsed = false;
	if (input->space && canUse && !(triggered && singleUse)) {
		canUse = false;
		if (hasColliderSet) {
			for (auto collision : collider->currentCollisions) {
				if (collision.gameObject->tag == "player") {
					if (!hasUsed) {
						hasUsed = true;
						bool passedRequirements = true;

						for (auto requirement : requirements) {
							passedRequirements = !requirement->requirementMet() ? false : passedRequirements;
							if (!passedRequirements) {
								log(requirement->getRequirementLog());
							}
						}

						if (passedRequirements) {

							triggered = true;

							use();

							if (message != "") {
								log(message);
							}
						}
					}
				}
			}
		}
	}

	if (!input->space) {
		canUse = true;
	}
}