#pragma once
#include "Component.h"

namespace TGM {

	const double GRAVITY = 1.0f;

	class Body : public Component {
	public:

		Vector force;
		double mass;

		Body(double _mass) {
			mass = _mass;
			zeroForces();
		}

		void zeroForces() {
			force = Vector::Zero();
		}

		void applyForce(Vector _force) {
			force += _force;
		}

		double calculateFrictionForce(double frictionCoef) {
			return frictionCoef * mass * GRAVITY;
		}

		Vector calculateAcceleration() {
			return force * (1 / mass);
		}

		Vector calculateVelocity(double dt) {
			return calculateAcceleration() * dt;
		}

		Vector calculateVelocity(double dt, Vector initialVelocity) {
			return calculateVelocity(dt) + initialVelocity;
		}

		Vector calculatePosition(double dt) {
			return calculateAcceleration() * dt * dt * 0.5;
		}

		Vector calculatePosition(double dt, Vector initialVelocity) {
			return calculatePosition(dt) + initialVelocity * dt;
		}

		Vector calculatePosition(double dt, Vector initialVelocity, Vector initialPosition) {
			return calculatePosition(dt, initialVelocity) + initialPosition;
		}
	};
};
