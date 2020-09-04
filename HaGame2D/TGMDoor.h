#pragma once
#include "HaGame2D.h"
#include "TGMEnum.h"

namespace TGM {

	class Door : public Component {
	public:
		bool open;
		Vector size;
		Color color;

		Door(Vector _size, Color _color, bool _open = false) {
			size = _size;
			color = _color;
			open = _open;
		}

		void update() {
			if (open) {
				if (transform->hasTag("WALL")) {
					transform->removeTag("WALL");
					transform->getComponent<SpriteRenderer>()->draw = false;
				}
			}
			else {
				if (!transform->hasTag("WALL")) {
					transform->addTag("WALL");
					transform->getComponent<SpriteRenderer>()->draw = true;
				}
			}
		}
	};
}