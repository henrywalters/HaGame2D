#pragma once
#include "HaGame2D.h"

namespace TGM {

	struct Bullet {
		float speed;
		float momentum;
		bool automatic;
	};

	const Bullet HandgunBullet = Bullet{
		500,
		25,
		false,
	};

	class Projectile : public Component {
	public:
		Vector velocity;
		float momentum;
		float distance;

		CollisionComponent* collider;

		Projectile(Vector _vel, float _momentum) {
			velocity = _vel;
			momentum = _momentum;
			distance = 0;
		}

		void onCreate() {
			collider = transform->getComponent<CollisionComponent>();
		}

		void update() {
			Vector delta = velocity * gameScene->dt_s();
			transform->move(delta);
			distance += delta.magnitude();

			for (auto coll : collider->currentCollisions) {
				if (coll.gameObject->hasTag("WALL") && !coll.gameObject->hasTag("ENEMY")) {
					gameScene->destroy(transform);
				}
			}

			if (distance > 800) {
				gameScene->destroy(transform);
			}
		}
	};
}