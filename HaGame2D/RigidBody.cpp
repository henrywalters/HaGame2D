#include "stdafx.h"
#include "HaGame2D.h"

bool RigidBody::hasForce(std::string name)
{
	return namedForces.find(name) != namedForces.end();
}

void RigidBody::addForce(std::string name, Vector force)
{
	if (hasForce(name)) {
		log("Force '" + name + "' already on rigid body.");
	}
	else {
		namedForces.insert(std::pair<std::string, Vector>(name, force));
	}
}

void RigidBody::applyForce(Vector force)
{
	appliedForce += force;
}

void RigidBody::handleCollisions()
{
	float dt = gameScene->dt_s();

	auto collider = transform->getComponent<BoxCollider>();

	if (collider != NULL) {
		for (auto collision : collider->currentCollisions) {

			log(collision.toString());

			if (collision.down && velocity.y >= 0) {
				velocity.y = 0;
				transform->move(Vector(0, -1 * collision.downOverlap));
			}

			if (collision.up && velocity.y <= 0) {
				velocity.y = 0;
			}
			
			if (collision.right && velocity.x >= 0) {
				velocity.x = 0;
			}

			if (collision.left && velocity.x) {
				velocity.x = 0;
			}
		}
	}
}

void RigidBody::sumForces()
{
	force += appliedForce;

	for (auto iter = namedForces.begin(); iter != namedForces.end(); ++iter) {
		force += iter->second;
	}
}

void RigidBody::handlePhysics()
{
	float dt = gameScene->dt_s();

	accel = force * props.mass;

	log(force.toString());

	velocity += accel * dt;
}
