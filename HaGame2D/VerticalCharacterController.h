#pragma once
#include "HaGame2D.h"

const float DEFAULT_WALK_FORCE = 10.0;
const float DEFAULT_JUMP_FORCE = 100.0;

class VerticalCharacterController : public Component
{

public:

	void onCreate() {

	}

	void update() {

		auto rigidBody = transform->getComponent<RigidBody>();

		if (rigidBody == NULL) {
			log("ERROR - Vertical Character Controller expects corresponding RigidBody component");
		}

		if (input->right) {
			log("Right");
			rigidBody->applyForce(Vector(DEFAULT_WALK_FORCE, 0));
		}

		if (input->left) {
			log("Left");
			rigidBody->applyForce(Vector(-1 * DEFAULT_WALK_FORCE, 0));
		}

		if (input->space) {
			rigidBody->applyForce(Vector(0, -1 * DEFAULT_JUMP_FORCE));
		}
	}
};

