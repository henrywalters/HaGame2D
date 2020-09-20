#pragma once
#include "Vector.h"
#include <unordered_map>

const double DEFAULT_GRAVITY = 10.0;
const double DEFAULT_MASS = 1.0;
const double DEFAULT_SPRING = 0.5;

struct RigidBodyParameters {
	double gravity;
	double mass;
	double spring;
};

const RigidBodyParameters DEFAULT_RB_PARAMETERS{
	DEFAULT_GRAVITY,
	DEFAULT_MASS,
	DEFAULT_SPRING,
};

class RigidBody : public Component
{
	const double epsilon = 3;

	RigidBodyParameters props;

	Vector force;
	Vector accel;
	Vector velocity;
	
	Vector appliedForce;
	std::unordered_map<std::string, Vector> namedForces;

	void handleCollisions();
	void sumForces();
	void handlePhysics();

public:

	RigidBody() {
		props = DEFAULT_RB_PARAMETERS;
	}

	// Checks if rigid body has named force.
	bool hasForce(std::string name);

	// Add a named force to a rigid body, for example, gravity;
	void addForce(std::string name, Vector force);

	// Apply a force for the current frame.
	void applyForce(Vector force);

	void onCreate() {
		velocity = Vector::Zero();
		addForce("gravity", Vector(0, props.gravity));
	}

	std::string toString() {
		return "F = " + force.toString() + " A = " + accel.toString() + " V = " + velocity.toString();
	}

	void update() {

		sumForces();
		handlePhysics();
		handleCollisions();

		transform->move(velocity);

		force = Vector::Zero();
		appliedForce = Vector::Zero();
	}
};

