#pragma once
#include "HaGame2D.h"

namespace TGM {

	const Vector HEALTHBAR_SIZE = Vector(80, 15);

	class HealthPickup : public Component {
	public:
		double health;

		HealthPickup(double _health) {
			health = _health;
		}
	};

	class HealthBar : public Component {
	public:
		double health;
		CircleCollider* collider;

		HealthBar(double initHealth = 100) {
			health = initHealth;
		}

		void doDamage(double dmg) {
			health -= dmg;
			health = Math::clamp(health, 0, 100);
		}

		void addHealth(double _health) {
			health += _health;
			health = Math::clamp(health, 0, 100);
		}

		bool isAlive() {
			return health > 0;
		}

		void onCreate() {
			collider = transform->getComponent<CircleCollider>();
		}

		void update() {
			double radius = collider->getCircle().radius;
			display->drawRect(
				transform->relativePosition.x + radius - (HEALTHBAR_SIZE.x / 2),
				transform->relativePosition.y - 25, 
				HEALTHBAR_SIZE.x, 
				HEALTHBAR_SIZE.y, Color(0, 0, 0, 255).rgb, 20);

			display->fillRect(
				transform->relativePosition.x + radius - (HEALTHBAR_SIZE.x / 2),
				transform->relativePosition.y - 25,
				HEALTHBAR_SIZE.x * (health / 100),
				HEALTHBAR_SIZE.y,
				Color(255, 0, 0, 100).rgb,
				19
			);
		}
	};
}