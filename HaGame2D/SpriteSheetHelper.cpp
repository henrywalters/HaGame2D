#include "stdafx.h"
#include "SpriteSheetHelper.h"
#include "HaGame2D.h"
#include "SpriteSheetLoader.h"
#include "Game.h"

const int scale = 2;

class FollowCursor : public BoxComponent {
public:

	float x = 0, y = 0;
	float screenWidth, screenHeight;
	const float precisionSpeed = .25;
	const float fullSpeed = 2;
	float speed = .5;

	float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	bool placedFirstPoint = false;
	bool placedSecondPoint = false;
	bool canProceed = true;
	bool canSave = true;

	bool menu = true;

	std::string tag;

	int cellIndex = 0;
	std::vector<SpriteSheetCell> cells;

	FollowCursor(int _width, int _height) {
		cells = std::vector<SpriteSheetCell>();
		width = _width;
		height = _height;
		screenWidth = _width;
		screenHeight = _height;
	}
	
	void viewMenu() {
		std::cout << "\nChoose an animation tag: ";
		std::cin >> tag;
		cellIndex = 0;
		menu = false;
		std::cout << "\n\nTo end Sprite Capture for: " << tag << " hit 1 to switch animation tag or 2 to save\n";
	}

	void captureSprite() {
		if (input->down) {
			y += speed;
		}
		if (input->up) {
			y -= speed;
		}
		if (input->right) {
			x += speed;
		}
		if (input->left) {
			x -= speed;
		}

		if (canProceed) {
			if (input->space && !placedFirstPoint) {
				canProceed = false;
				placedFirstPoint = true;
				x1 = x; y1 = y;
			}
			else if (input->space && !placedSecondPoint) {
				x2 = x; y2 = y;
				canProceed = false;
				placedSecondPoint = true;
				
			}
		}

		if (!input->space) {
			canProceed = true;
			if (placedFirstPoint && placedSecondPoint) {
				placedFirstPoint = false; placedSecondPoint = false;
				cells.push_back(SpriteSheetCell{ tag, cellIndex, x1 / scale, y1 / scale, (x2 - x1) / scale, (y2 - y1) / scale });
				cellIndex += 1;
				std::cout << "Captured Sprite: " << tag << "_" << cellIndex << "\n";
			}
		}

		if (placedFirstPoint) {
			display->drawLine(0, y1, screenWidth, y1, Color::blue());
			display->drawLine(x1, 0, x1, screenHeight, Color::blue());
		}

		display->drawLine(0, y, screenWidth, y, Color::green(), 17);
		display->drawLine(x, 0, x, screenHeight, Color::green());
	}

	void update() {

		if (input->one) {
			menu = true;
		}

		if (input->shift) {
			speed = fullSpeed;
		}
		else {
			speed = precisionSpeed;
		}

		if (input->two && canSave) {
			std::cout << "\nSaving To: ";
			std::string filePath;
			std::cin >> filePath;
			SpriteSheetLoader::save(filePath.c_str(), cells);
			//cells.clear();
			menu = true;
			canSave = false;
		}

		if (!input->two) {
			canSave = true;
		}

		if (menu) {
			viewMenu();
		}
		else {
			captureSprite();
		}
	}
};

SpriteSheetHelper::SpriteSheetHelper(int width, int height)
{

	float scaledWidth = scale * width;
	float scaledHeight = scale * height;

	Game game = Game(scaledWidth, scaledHeight);
	Scene helper = *game.addScene("helper");
	helper.setDisplayPort(0, 0, scaledWidth, scaledHeight);

	auto sheet = helper.add();
	sheet->addComponent(new SpriteRenderer("../Assets/mapbuilder-tiles.png", scaledWidth, scaledHeight, NULL));
	auto cursor = sheet->addComponent(new FollowCursor(scaledWidth, scaledHeight));

	game.prepareScene();
	
	while (game.running) {
		game.tick();
	}
}


SpriteSheetHelper::~SpriteSheetHelper()
{
}
