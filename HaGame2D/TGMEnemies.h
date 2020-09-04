#pragma once
#include "HaGame2D.h"
#include "CollisionSystem.h"
#include "TGMEnum.h";

namespace TGM {

	class SliderObstacle : public Component {
	public:
		float speed;
		Direction direction;

		SliderObstacle(float _speed, Direction _direction) {
			speed = _speed;
			direction = _direction;
		}

		void update() {
			if (this->direction == Direction::Horizontal) {
				transform->move(Vector(speed * gameScene->dt_s(), 0));
			}
			else {
				transform->move(Vector(0, speed * gameScene->dt_s()));
			}
		}
	};

	const float ENEMY_SPEED = 75;

	const float MAX_ATTACK_DISTANCE = 500;
	const float MIN_ATTACK_DISTANCE = 150;

	const float MAX_PURSUE_DISTANCE = 1000;
	const float MIN_PURSUE_DISTANCE = 600;

	const float MAX_ATTACK_RATE = 2;
	const float MIN_ATTACK_RATE = 20;

	enum EnemyState {
		Idle,
		Pursuing,
		Attacking,
	};

	class Enemy : public Component {
	public:
		float attackRate;
		float timeSinceLastAttack = 0.0;
		EnemyState state = EnemyState::Idle;

		float attackDistance;
		float pursueDistance;

		void onCreate() {
			attackRate = Random::number(MAX_ATTACK_RATE, MIN_ATTACK_RATE) / 10.0f;
			
			attackDistance = Random::number(MIN_ATTACK_DISTANCE, MAX_ATTACK_DISTANCE);
			pursueDistance = Random::number(MIN_PURSUE_DISTANCE, MAX_PURSUE_DISTANCE);
			std::cout << "#### NEW ENEMY ####\n";
			std::cout << "Attack Rate: " << attackRate << std::endl;
			std::cout << "Pursue Dist: " << pursueDistance << std::endl;
			std::cout << "Attack Dist: " << attackDistance << std::endl;
		}

		void update() {
			timeSinceLastAttack += gameScene->dt_s();
		}

		bool attack() {
			if (timeSinceLastAttack > attackRate) {
				timeSinceLastAttack = 0;
				return true;
			}
			return false;
		}
	};

	class EnemySystem : public System {

		Scene* scene;

		std::vector<GameObject*> enemies;

		CollisionSystem* collisionSystem;

		GameObject* addBullet(Box rect, Vector velocity, float rotation, float momentum) {
			auto bullet = scene->add()
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/enemyBullet.png", rect.width, rect.height, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->setPosition(Vector(rect.x, rect.y))
				->addTagAnd("ENEMY_BULLET")
				->addComponentAnd(new TGM::Projectile(velocity, momentum));

			bullet->rotate(rotation);
			bullet->getComponent<CollisionComponent>()->pollCollisions = true;

			scene->instantiate(bullet);

			return bullet;
		}

	public:
		EnemySystem(CollisionSystem* _collisionSystem) : System("Enemy_System") {
			collisionSystem = _collisionSystem;
		};

		void onInit() {
			scene = getScene();
		}

		void update() {
			enemies = scene->getGameObjectsWhere<Enemy>();
			auto player = scene->getGameObjectsWhereHasTag("PLAYER")[0];

			for (auto enemy : enemies) {
				Enemy* component = enemy->getComponent<Enemy>();
				Vector playerDelta = enemy->position - player->position;
				float distance = playerDelta.magnitude();

				if (distance > component->pursueDistance) {
					component->state = EnemyState::Idle;
				}
				else if (distance <= component->pursueDistance && distance > component->attackDistance) {
					component->state = EnemyState::Pursuing;
					Vector vel = playerDelta.normalized() * ENEMY_SPEED * scene->dt_s() * -1;
					adjustVelocityForBoundaries(enemy->uid, enemy->getComponent<CircleCollider>(), vel, scene->dt_s());
					component->transform->move(vel);
				}
				else {
					component->state = EnemyState::Attacking;
					Vector vel = playerDelta.normalized() * HandgunBullet.speed * -1;
					if (component->attack()) {
						addBullet(Box{ enemy->position.x, enemy->position.y, 20, 20 }, vel, 0, 15);
					}
				}

				for (auto collision : enemy->getComponent<CollisionComponent>()->currentCollisions) {
					if (collision.gameObject->hasTag("PLAYER_BULLET")) {
						enemy->getComponent<HealthBar>()->doDamage(collision.gameObject->getComponent<Projectile>()->momentum);
						if (!enemy->getComponent<HealthBar>()->isAlive()) {
							scene->destroy(enemy);
						}
						collision.gameObject->getComponent<CollisionComponent>()->active = false;
					}
				}
			}
		}

		Circle adjustedCircle(CircleCollider* collider, Vector delta) {
			Circle adjusted;
			adjusted = collider->getCircle();
			adjusted.center += delta;
			return adjusted;
		}

		bool isCollidingIf(int uid, CircleCollider* collider, Vector delta) {
			auto circle = adjustedCircle(collider, delta);
			auto collisions = collisionSystem->checkCircleCollisions(uid, circle, getScene()->getGameObjectsWhere<CollisionComponent>());
			for (auto col : collisions) {
				if (col.gameObject->hasTag("WALL")) {
					return true;
				}
			}
			return false;
		}

		void adjustVelocityForBoundaries(int uid, CircleCollider* collider, Vector &velocity, float dt) {
			if (velocity.x != 0 && isCollidingIf(uid, collider, Vector(velocity.x, 0))) velocity.x = 0;
			if (velocity.y != 0 && isCollidingIf(uid, collider,  Vector(0, velocity.y))) velocity.y = 0;
		}

	};
}