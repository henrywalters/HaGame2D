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

const double RUN_SPEED = 5.0f;
const double SPEED = 10.0f;

const double RUN_FORCE = 250.0f;
const double WALK_FORCE = 300.0f;

const double FRICTION_COEF = 15.0f;

const double ROT_ACCEL = 0.6f;
const double ROT_DEACCEL = 1.0f;
const double MAX_ROT_VEL = 0.1f;

const double EPSILON = 0.3f;

const std::string BULLET_PATH = "Assets/Sprites/bullet.png";

const double SHOOT_DELAY = 0.2;

namespace TGM {

	class PlayerController : public System {

		double speed;
		double dt;

		Vector velocity;
		double rotVelocity;

		Scene* scene;
		Input* input;
		

		TGM::Body* body;

		Vector playerSize;

		CollisionSystem* collisionSystem;
		TGM::TriggerReceiverSystem* trSystem;

		Vector chunkIndex;

		CircleCollider* collider;
		TGM::HealthBar* healthBar;

		double health = 100;

		double timeSinceLastShot = 0;
		bool canShoot = true;

		Bullet weaponType = HandgunBullet;

		Gamepad *gamepad;

		Vector center() {
			return player->position + player->origin + Vector(playerSize.x / 2.0f, playerSize.y / 2.0f);
		}

		GameObject* addBullet(Scene* scene, Box rect, Vector velocity, double rotation, double momentum) {
			auto bullet = (new GameObject())
				->addComponentAnd(new SpriteRenderer(BULLET_PATH, rect.width, rect.height, NULL))
				->addComponentAnd(new BoxCollider(rect.width, rect.height))
				->setPosition(Vector(rect.x, rect.y))
				->addTagAnd("PLAYER_BULLET")
				->addComponentAnd(new TGM::Projectile(velocity, momentum));

			bullet->rotate(rotation);
			bullet->getComponent<CollisionComponent>()->pollCollisions = true;

			scene->instantiate(bullet);

			return bullet;
		}

	public:
		std::function<void()> onLevelComplete = []() {};
		std::function<void()> onDead = []() {};
		GameObject* player;
		
		PlayerController(CollisionSystem* _collisionSystem, TriggerReceiverSystem* _trSystem) : System("Player Controller") {
			trSystem = _trSystem;
			gamepad = NULL;
			collisionSystem = _collisionSystem;
			chunkIndex = Vector::Zero();
		}

		void onInit() {
			scene = getScene();
			player = scene->getGameObjectsWhereHasTag("PLAYER")[0];
			input = scene->input;

			gamepad = new Gamepad(0);

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
						player->move(velocity * -50 * scene->dt_s());
					}

					if (coll.gameObject->hasTag("SLIDER_OBSTACLE")) {
						// Handle Damage
					}

					if (coll.gameObject->hasTag("TRIGGER") && (gamepad->aPressed || input->actionDown)) {
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
						//std::cout << "Hit by " << coll.gameObject->uid << std::endl;
						//std::cout << "Hit for " << coll.gameObject->getComponent<Projectile>()->momentum << std::endl;
						if (coll.gameObject->getComponent<CollisionComponent>()->active) {
							healthBar->doDamage(coll.gameObject->getComponent<Projectile>()->momentum);
							if (!healthBar->isAlive()) {
								onDead();
							}
						}
						coll.gameObject->getComponent<CollisionComponent>()->active = false;
						// scene->destroy(coll.gameObject);

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
			gamepad->pollDevice();
			dt = scene->dt_s();
			double radius = collider->getCircle().radius;

			// Movement portion
			body->zeroForces();
			Vector dir = Vector::Zero();

			double forceMag = input->shift ? RUN_FORCE : WALK_FORCE;

			
			if (input->up) dir.y = -1;
			if (input->down) dir.y = 1;
			if (input->left) dir.x = -1;
			if (input->right) dir.x = 1;
			
			/*if (gamepad->lAxis.magnitude() > 0.4) {
				dir = gamepad->lAxis.normalized();
			}*/
			
			double speed = velocity.magnitude();

			if (speed < (input->shift ? RUN_SPEED : SPEED)) {
				body->applyForce(dir.normalized() * forceMag);
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
			//std::cout << "f(" << gamepad->rAxis.toString() << ") = " << atan2(gamepad->rAxis.x, -gamepad->rAxis.y) << std::endl;

			auto mouseAngle = atan2(mouseDelta.x, -mouseDelta.y);
			player->setRotation(mouseAngle);

			std::cout << rotVelocity << std::endl;
			
			player->rotate(rotVelocity);

			// Shooting

			if (input->one) {
				weaponType = HandgunBullet;
			}

			if (input->two) {
				weaponType = ShotgunBullet;
			}

			if (input->three) {
				weaponType = MachineGunBullet;
			}

			if (input->four) {
				weaponType = SMGBullet;
			}

			if (!canShoot) {
				timeSinceLastShot += dt;
				if (timeSinceLastShot > weaponType.delay) {
					canShoot = true;
					timeSinceLastShot = 0;
				}
			}

			if (((canShoot && weaponType.automatic && input->fire1) || input->fire1Down)) {

				// gamepad->rumble(0.8, 50);

				canShoot = false;
				Vector pos = Vector(player->position.x + radius, player->position.y + radius);
				pos += Vector(radius * cos(player->rotation - M_PI / 2), radius * sin(player->rotation - M_PI / 2));
				
				for (int i = 0; i < weaponType.projectiles; i++) {
					double rotation = player->rotation;
					if (weaponType.spread > 0) {
						rotation += ( M_PI * Random::number(-(weaponType.spread / 2) * 1000, (weaponType.spread / 2) * 1000) / 1000.0) / 180.0;
					}
					Vector bulletVelocity = Vector(weaponType.speed * cos(rotation - M_PI / 2), weaponType.speed * sin(rotation - M_PI / 2));
					addBullet(scene, Box{ pos.x, pos.y, weaponType.projectileSize.x, weaponType.projectileSize.y }, bulletVelocity, rotation, weaponType.momentum);
				}
				
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

		void adjustVelocityForBoundaries(double dt) {
			if (velocity.x != 0 && isCollidingIf(Vector(velocity.x, 0))) velocity.x = 0;
			if (velocity.y != 0 && isCollidingIf(Vector(0, velocity.y))) velocity.y = 0;
		}
	};
};