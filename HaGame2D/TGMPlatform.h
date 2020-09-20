#pragma once
#include "HaGame2D.h"
#include "TGMEnum.h"

namespace TGM {

	const double SLIDER_WIDTH = 15;

	class MovingPlatform : public Component {

	public:
		Vector currentVelocity;
		bool moving;
		double length;
		Color platformColor;
		Color sliderColor;
		Direction direction;
		BoxComponent* boxC;
		BoxComponent* platformBoxC;
		double speed;

		MovingPlatform(Color _platformColor, Color _sliderColor, Direction _direction, double _speed, bool _moving = true) {
			platformColor = _platformColor;
			sliderColor = _sliderColor;
			direction = _direction;
			speed = _speed;
			moving = _moving;
		}

		void onCreate() {
			boxC = transform->getComponent<BoxComponent>();
			platformBoxC = transform->getComponentInChildren<BoxComponent>();
			if (direction == Direction::Horizontal) currentVelocity = Vector(speed, 0);
			else currentVelocity = Vector(0, speed);
		}

		void update() {
			double dt = gameScene->dt_s();
			
			if (direction == Direction::Horizontal) {
				display->fillRect(
					transform->relativePosition.x,
					transform->relativePosition.y + boxC->relativeHeight / 2.0f - SLIDER_WIDTH / 2.0f,
					boxC->relativeWidth,
					SLIDER_WIDTH,
					sliderColor.rgb,
					9
				);

				if (moving) {
					if (
						!(currentVelocity.x > 0 && platformBoxC->transform->position.x + platformBoxC->width + currentVelocity.x * dt < transform->position.x + boxC->width) &&
						!(currentVelocity.x < 0 && platformBoxC->transform->position.x - currentVelocity.x * dt > transform->position.x)
						) currentVelocity = currentVelocity * -1;

					platformBoxC->transform->move(currentVelocity * dt);
				}

				display->fillRect(
					platformBoxC->transform->relativePosition.x,
					transform->relativePosition.y + boxC->relativeHeight / 2.0f - platformBoxC->relativeHeight / 2.0f,
					platformBoxC->relativeWidth,
					platformBoxC->relativeHeight,
					platformColor.rgb
				);
				
			}
			else {
				display->fillRect(
					transform->relativePosition.x + boxC->relativeWidth / 2.0f - SLIDER_WIDTH / 2.0f,
					transform->relativePosition.y,
					SLIDER_WIDTH,
					boxC->relativeHeight,
					sliderColor.rgb,
					9
				);

				if (moving) {
					if (
						!(currentVelocity.y > 0 && platformBoxC->transform->position.y + platformBoxC->height + currentVelocity.y * dt < transform->position.y + boxC->height) &&
						!(currentVelocity.y < 0 && platformBoxC->transform->position.y - currentVelocity.y * dt > transform->position.y)
						) currentVelocity = currentVelocity * -1;

					platformBoxC->transform->move(currentVelocity * dt);
				}

				display->fillRect(
					transform->relativePosition.x + boxC->relativeWidth / 2.0f - platformBoxC->relativeWidth / 2.0f,
					platformBoxC->transform->relativePosition.y,
					platformBoxC->relativeWidth,
					platformBoxC->relativeHeight,
					platformColor.rgb
				);
			}
		}
	};
}