#pragma once
#include "HaGame2D.h"
#include "TGMTriggerReceiver.h"

namespace TGM {

	const float LEVER_PADDING = 10;

	class LeverDisplay : public Component {
	public:
		Color color;
		Color offColor;
		Color onColor;

		GameObject* parent;
		Trigger* trigger;

		BoxComponent* boxC;

		LeverDisplay(Color _color, Color _offColor, Color _onColor) {
			color = _color;
			offColor = _offColor;
			onColor = _onColor;
		}

		void onCreate() {
			std::cout << "INITIALIED LEVER DISPLAY" << std::endl;
			parent = transform->parentGameObject;
			trigger = parent->getComponentInChildren<Trigger>();
			boxC = parent->getComponentInChildren<BoxComponent>();
		}

		void update() {

			// std::cout << "Parent relative: " << parent->relativePosition.toString() << " abs: " << parent->position.toString() << std::endl;
			// std::cout << "Child relative:  " << transform->relativePosition.toString() << " abs: " << transform->position.toString() << std::endl;

			display->fillRect(transform->relativePosition.x, transform->relativePosition.y, boxC->relativeWidth, boxC->relativeHeight, color.rgb, 9);

			if (trigger->value == 1) {
				display->fillRect(
					transform->relativePosition.x + LEVER_PADDING, 
					transform->relativePosition.y + LEVER_PADDING, 
					boxC->relativeWidth - LEVER_PADDING * 2, 
					boxC->relativeHeight - LEVER_PADDING * 2,
					onColor.rgb
				);
			}
			else {
				display->fillRect(
					transform->relativePosition.x + LEVER_PADDING,
					transform->relativePosition.y + LEVER_PADDING,
					boxC->relativeWidth - LEVER_PADDING * 2,
					boxC->relativeHeight - LEVER_PADDING * 2,
					offColor.rgb
				);
			}
		}
	};
}