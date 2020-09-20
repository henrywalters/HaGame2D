#pragma once
#include "HaGame2D.h"

namespace TGM {

	struct Bullet {
		int projectiles;
		Vector projectileSize;
		double spread;
		double speed;
		double momentum;
		bool automatic;
		double delay;
		int capacity;
	};

	const Bullet EnemyBullet = Bullet{
		1,
		Vector(20, 20),
		0,
		250,
		12.5,
		false,
		0,
		0,
	};

	const Bullet HandgunBullet = Bullet{
		1,
		Vector(10, 10),
		0,
		1000,
		25,
		false,
		0.2,
		100,
	};

	const Bullet ShotgunBullet = Bullet{
		20,
		Vector(5, 5),
		30,
		1200,
		6.0,
		false,
		0.4,
		36,
	};

	const Bullet MachineGunBullet = Bullet{
		1,
		Vector(5, 5),
		5,
		1500,
		20,
		true,
		0.2,
		240,
	};

	const Bullet SMGBullet = Bullet{
		1,
		Vector(5, 5),
		10,
		1500,
		5,
		true,
		0.08,
		360,
	};

	class Projectile : public Component {
	public:
		Vector velocity;
		double momentum;
		double distance;

		CollisionComponent* collider;

		Projectile(Vector _vel, double _momentum) {
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