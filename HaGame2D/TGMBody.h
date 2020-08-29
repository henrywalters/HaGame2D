#pragma once
#include "Component.h"

namespace TGM {
	class BodyComponent : public Component {
	public:

		float mass;

		BodyComponent(float _mass) {
			mass = _mass;
		}
	};
};
