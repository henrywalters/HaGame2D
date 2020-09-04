#pragma once
#include "HaGame2D.h"
#include "TGMBody.h"
#include "CollisionSystem.h"
#include "TGMTriggerReceiver.h"
#include "TGMDoor.h"
#include "math.h"
#include "OLC_2020_TheGreatMachine.h"
#include "TGMPlatform.h"
#include "TGMProjectile.h"
#include "TGMHealthBar.h"

const float RUN_SPEED = 5.0f;
const float SPEED = 10.0f;

const float RUN_FORCE = 250.0f;
const float WALK_FORCE = 300.0f;

const float FRICTION_COEF = 15.0f;

const float ROT_ACCEL = 0.1f;
const float ROT_DEACCEL = 1.0f;
const float MAX_ROT_VEL = 2.0f;

const float EPSILON = 0.05f;

namespace TGM {

	class PlayerController : public System {

		float speed;
		float dt;

		Vector velocity;
		float rotVelocity;

		Scene* scene;
		Input* input;
		

		TGM::Body* body;

		Vector playerSize;

		CollisionSystem* collisionSystem;
		TGM::TriggerReceiverSystem* trSystem;

		Vector chunkIndex;

		CircleCollider* collider;
		TGM::HealthBar* healthBar;

		float health = 100;

		Vector center() {
			return player->position + player->origin + Vector(playerSize.x / 2.0f, playerSize.y / 2.0f);
		}

		GameObject* addBullet(Scene* scene, Box rect, Vector velocity, float rotation, float momentum) {
			auto bullet = scene->add()
				->addComponentAnd(new SpriteRenderer("../Assets/TGM/Sprites/bullet.png", rect.width, rect.height, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->setPosition(Vector(rect.x, rect.y))
				->addTagAnd("PLAYER_BULLET")
				->addComponentAnd(new TGM::Projectile(velocity, momentum));

			bullet->rotate(rotation);

			scene->instantiate(bullet);

			return bullet;
		}

	public:
		std::function<void()> onLevelComplete = []() {};
		GameObject* player;
		PlayerController(CollisionSystem* _collisionSystem, TriggerReceiverSystem* _trSystem) : System("Player Controller") {
			trSystem = _trSystem;
			collisionSystem = _collisionSystem;
			chunkIndex = Vector::Zero();
		}

		void onInit() {
			scene = getScene();
			player = scene->getGameObjectsWhereHasTag("PLAYER")[0];
			input = scene->input;

			body = player->getComponent<TGM::Body>();
			if (body == NULL) throw new std::exception("TGM::BodyComponent required on player");

			collider = player->getComponent<CircleCollider>();
			if (collider == NULL) throw new std::exception("Player must have a circle collider component");

			healthBar = player->getComponent <HealthBar>();

			velocity = Vector::Zero();
			rotVelocity = 0;

			collisionSystem->events.unsubscribeAll();

			collisionSystem->events.subscribe([this](Collision coll) {
				if (coll.sourceUid == player->uid) {
					if (coll.gameObject->hasTag("WALL")) {
						player->move(velocity * -10 * scene->dt_s());
					}

					if (coll.gameObject->hasTag("SLIDER_OBSTACLE")) {
						// Handle Damage
					}

					if (coll.gameObject->hasTag("TRIGGER") && input->actionDown) {
						trSystem->trigger(coll.gameObject->uid, coll.gameObject->getComponent<Trigger>()->value == 1 ? 0 : 1);
					}

					if (coll.gameObject->hasTag("PLATFORM")) {
						auto platform = coll.gameObject->parentGameObject->getComponent<MovingPlatform>();
						if (platform->moving) {
							std::cout << "On Platform moving " << platform->currentVelocity.toString() << std::endl;
							velocity += platform->currentVelocity;
							std::cout << "New Velocity = " << velocity.toString() << std::endl;
						}

					}

					if (coll.gameObject->hasTag("HEALTH")) {
						healthBar->addHealth(coll.gameObject->getComponent<HealthPickup>()->health);
						scene->destroy(coll.gameObject);
					}

					if (coll.gameObject->hasTag("ENEMY_BULLET")) {
						healthBar->doDamage(coll.gameObject->getComponent<Projectile>()->momentum);
						if (!healthBar->isAlive()) {
							// GAME OVER
						}
						coll.gameObject->getComponent<CollisionComponent>()->active = false;

					}

					if (coll.gameObject->hasTag("END")) {
						onLevelComplete();
					}
				}
			});
		}

		void initialize() {
			
		}

		void update() {
			dt = scene->dt_s();
			float radius = collider->getCircle().radius;

			// Movement portion
			body->zeroForces();
			Vector dir = Vector::Zero();

			float forceMag = input->shift ? RUN_FORCE : WALK_FORCE;

			if (input->up) dir.y = -1;
			if (input->down) dir.y = 1;
			if (input->left) dir.x = -1;
			if (input->right) dir.x = 1;

			float speed = velocity.magnitude();

			if (speed < (input->shift ? RUN_SPEED : SPEED)) {
				body->applyForce(dir.normalized() * forceMag);
				player->rotate(dir.x * dt * 5);
			}

			if (speed > EPSILON) {
				body->applyForce(velocity.normalized() * -1 * body->calculateFrictionForce(FRICTION_COEF));
			}

			Vector accel = body->calculateAcceleration();

			auto nextSpeed = (velocity + (accel * dt)).magnitude();

			velocity += accel * dt;

			adjustVelocityForBoundaries(dt);
			
			if (velocity.magnitude() < EPSILON) {
				velocity = Vector::Zero();
			}

			player->move(velocity);

			Vector newChunkIndex = Vector(floor(player->position.x / scene->screenWidth), floor(player->position.y / scene->screenHeight));

			if (newChunkIndex != chunkIndex) {
				// chunkIndex = newChunkIndex;
				// std::cout << "Chunk: " << chunkIndex.toString() << std::endl;
				// scene->camera.lerp(Vector(chunkIndex.x * scene->screenWidth, chunkIndex.y * scene->screenHeight), 60);
			}

			scene->camera.positionCenter(player->position);

			// Rotation portion

			auto mouseDelta = input->globalMousePos() - center();
			auto mouseAngle = atan2(mouseDelta.y, mouseDelta.x);

			player->setRotation(mouseAngle + (M_PI / 2));

			// Shooting

			if (input->fire1Down) {
				
				Vector pos = Vector(player->position.x + radius, player->position.y + radius);
				pos += Vector(radius * cos(player->rotation - M_PI / 2), radius * sin(player->rotation - M_PI / 2));
				Vector bulletVelocity = Vector(HandgunBullet.speed * cos(player->rotation - M_PI / 2), HandgunBullet.speed * sin(player->rotation - M_PI / 2));
				addBullet(scene, Box{ pos.x, pos.y, 20, 20 }, bulletVelocity, player->rotation, HandgunBullet.momentum);
			}
		}

		Circle adjustedCircle(Vector delta) {
			Circle adjusted;
			adjusted = collider->getCircle();
			adjusted.center += delta;
			return adjusted;
		}

		bool isCollidingIf(Vector delta) {
			auto circle = adjustedCircle(delta);
			auto collisions = collisionSystem->checkCircleCollisions(player->uid, circle, getScene()->getGameObjectsWhere<CollisionComponent>());
			for (auto col : collisions) {
				if (col.gameObject->hasTag("WALL")) {
					return true;
				}
			}
			return false;
		}

		void adjustVelocityForBoundaries(float dt) {
			if (velocity.x != 0 && isCollidingIf(Vector(velocity.x, 0))) velocity.x = 0;
			if (velocity.y != 0 && isCollidingIf(Vector(0, velocity.y))) velocity.y = 0;
		}
	};
};