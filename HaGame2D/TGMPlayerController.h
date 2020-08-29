#pragma once
#include "HaGame2D.h"
#include "TGMBody.h";

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
		float rotVelocity;

		Scene* scene;
		Input* input;
		GameObject* player;
		float playerMass;
		Vector playerSize;

		Vector center() {
			return player->position + Vector(playerSize.x / 2.0f, playerSize.y / 2.0f);
		}

	public:
		PlayerController(GameObject* _player) : System("Player Controller") {
			player = _player;
		}

		void onInit() {
			scene = getScene();
			input = scene->input;

			SpriteRenderer* spriteRenderer = player->getComponent<SpriteRenderer>();
			if (spriteRenderer != NULL) playerSize = Vector(spriteRenderer->width, spriteRenderer->height);

			TGM::BodyComponent* body = player->getComponent<TGM::BodyComponent>();
			if (body == NULL) throw new std::exception("TGM::BodyComponent required on player");

			playerMass = body->mass;
			velocity = Vector::Zero();
			rotVelocity = 0;
		}

		void update() {
			dt = scene->dt_s();

			// Movement portion
			Vector accel = Vector::Zero();

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
			
			player->move(velocity * dt);

			// Rotation portion

			auto mouseDelta = input->mousePos() - center();
			auto mouseAngle = atan(mouseDelta.y / mouseDelta.x);

			player->setRotation(mouseAngle);
		}
	};
};