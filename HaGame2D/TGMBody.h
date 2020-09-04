#pragma once
#include "Component.h"

namespace TGM {

	const float GRAVITY = 1.0f;

	class Body : public Component {
	public:

		Vector force;
		float mass;

		Body(float _mass) {
			mass = _mass;
			zeroForces();
		}

		void zeroForces() {
			force = Vector::Zero();
		}

		void applyForce(Vector _force) {
			force += _force;
		}

		float calculateFrictionForce(float frictionCoef) {
			return frictionCoef * mass * GRAVITY;
		}

		Vector calculateAcceleration() {
			return force * (1 / mass);
		}

		Vector calculateVelocity(float dt) {
			return calculateAcceleration() * dt;
		}

		Vector calculateVelocity(float dt, Vector initialVelocity) {
			return calculateVelocity(dt) + initialVelocity;
		}

		Vector calculatePosition(float dt) {
			return calculateAcceleration() * dt * dt * 0.5;
		}

		Vector calculatePosition(float dt, Vector initialVelocity) {
			return calculatePosition(dt) + initialVelocity * dt;
		}

		Vector calculatePosition(float dt, Vector initialVelocity, Vector initialPosition) {
			return calculatePosition(dt, initialVelocity) + initialPosition;
		}
	};
};
