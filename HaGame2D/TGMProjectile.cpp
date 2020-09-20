#include "stdafx.h"
#include "TGMProjectile.h"

namespace TGM {
	GameObject* addBullet(Scene* scene, Box rect, Vector velocity, double rotation, double momentum) {
		auto bullet = scene->add()
			->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/bullet.png", rect.width, rect.height, NULL))
			->addComponentAnd(new BoxCollider(rect.width, rect.height))
			->setPosition(Vector(rect.x, rect.y))
			->addComponentAnd(new TGM::Projectile(velocity, momentum));

		bullet->rotate(rotation);

		return bullet;
	}
}