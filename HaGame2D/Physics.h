#pragma once
#include "Vector.h"

class Physics
{
public:

	static Vector calcForce(double mass, Vector accel) {
		return accel * mass;
	}

	static Vector calcKineticEnergy(double mass, Vector velocity) {
		Vector vSq = Vector(velocity.x * velocity.x, velocity.y * velocity.y, velocity.z * velocity.z);
		return vSq * mass * 0.5;
	}
};

