#pragma once
#include "HaGame2D.h"
#include "TGMBody.h";
#include "CollisionSystem.h"

const float RUN_SPEED = 40.0f;
const float SPEED = 15.0f;

const float RUN_ACCEL = 20.0f;
const float ACCEL = 10.0f;
const float RUN_DEACCEL = 30.0f;
const float DEACCEL = 15.0f;

const float FRICTION_COEF = 1.0f;

const float ROT_ACCEL = 0.1f;
const float ROT_DEACCEL = 1.0f;
const float MAX_ROT_VEL = 2.0f;

namespace TGM {

	class PlayerController : public System {

		float speed;
		float accelMag;
		float deaccel;
		float dt;

		Vector velocity;
		Vector accel;
		float rotVelocity;

		Scene* scene;
		Input* input;
		GameObject* player;
		float playerMass;
		Vector playerSize;

		CollisionSystem* collisionSystem;

		CircleCollider* collider;

		Vector center() {
			return player->position + Vector(playerSize.x / 2.0f, playerSize.y / 2.0f);
		}

	public:
		PlayerController(GameObject* _player, CollisionSystem* _collisionSystem) : System("Player Controller") {
			player = _player;
			collisionSystem = _collisionSystem;
		}

		void onInit() {
			scene = getScene();
			input = scene->input;

			SpriteRenderer* spriteRenderer = player->getComponent<SpriteRenderer>();
			if (spriteRenderer != NULL) playerSize = Vector(spriteRenderer->width, spriteRenderer->height);

			TGM::BodyComponent* body = player->getComponent<TGM::BodyComponent>();
			if (body == NULL) throw new std::exception("TGM::BodyComponent required on player");

			collider = player->getComponent<CircleCollider>();
			if (collider == NULL) throw new std::exception("Player must have a circle collider component");

			playerMass = body->mass;
			velocity = Vector::Zero();
			rotVelocity = 0;

			collisionSystem->events.subscribe([this](Collision coll) {
				std::cout << "Colliding with: " << coll.gameObject->uid << " moving " << velocity.toString() << "m/s" << std::endl;
				player->move(velocity * -5 * scene->dt_s());
			});
		}

		void update() {
			dt = scene->dt_s();

			// Movement portion
			accel = Vector::Zero();

			accelMag = input->shift ? RUN_ACCEL : ACCEL;
			deaccel = input->shift ? RUN_DEACCEL : DEACCEL;

			if (input->up) {
				if (velocity.y > 0) accel.y += -deaccel;
				else accel.y += -accelMag;
			}

			if (input->down) {
				if (velocity.y < 0) accel.y += deaccel;
				else accel.y += accelMag;
			}

			if (input->left) {
				if (velocity.x > 0) accel.x += -deaccel;
				else accel.x += -accelMag;
			}

			if (input->right) {
				if (velocity.x < 0) accel.x += deaccel;
				else accel.x += accelMag;
			}

			accel += (velocity * -1) * (FRICTION_COEF * playerMass * dt);

			auto nextSpeed = (velocity + (accel * dt)).magnitude();

			if (nextSpeed < input->shift ? RUN_SPEED : SPEED) {
				velocity += accel * dt;
			}

			adjustVelocityForBoundaries(dt);
			
			player->move(velocity * dt);

			// Rotation portion

			auto mouseDelta = input->mousePos() - center();
			auto mouseAngle = atan(mouseDelta.y / mouseDelta.x);

			player->setRotation(mouseAngle);
		}

		Circle adjustedCircle(Vector delta) {
			Circle adjusted;
			adjusted = collider->getCircle();
			adjusted.center += delta;
			return adjusted;
		}

		bool isCollidingIf(Vector delta) {
			auto circle = adjustedCircle(delta);
			return collisionSystem->checkCircleCollisions(player->uid, circle, getScene()->getGameObjectsWhere<CollisionComponent>()).size() > 0;
		}

		void adjustVelocityForBoundaries(float dt) {
			if (velocity.x != 0 && isCollidingIf(Vector(velocity.x * dt, 0))) velocity.x = 0;
			if (velocity.y != 0 && isCollidingIf(Vector(0, velocity.y * dt))) velocity.y = 0;
		}
	};
};