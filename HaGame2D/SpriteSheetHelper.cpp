#include "stdafx.h"
#include "SpriteSheetHelper.h"
#include "HaGame2D.h"
#include "SpriteSheetLoader.h"
#include "Game.h"

const int scale = 3;

class FollowCursor : public BoxComponent {
public:

	double x = 0, y = 0;
	double screenWidth, screenHeight;
	const double precisionSpeed = .25;
	const double fullSpeed = 2;
	double speed = .5;

	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
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
			SpriteSheetLoader::save(filePath, cells);
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

SpriteSheetHelper::SpriteSheetHelper(std::string spritePath, int width, int height)
{
	double scaledWidth = scale * width;
	double scaledHeight = scale * height;

	Game game = Game(scaledWidth, scaledHeight);
	Scene helper = *game.addScene("helper");
	helper.setDisplayPort(0, 0, scaledWidth, scaledHeight);

	auto sheet = helper.add();
	sheet->addComponent(new SpriteRenderer(spritePath, scaledWidth, scaledHeight, NULL));
	auto cursor = sheet->addComponent(new FollowCursor(scaledWidth, scaledHeight));

	game.prepareScene();
	
	while (game.running) {
		game.tick();
	}
}


SpriteSheetHelper::~SpriteSheetHelper()
{
}

void SpriteSheetHelper::autoLoad(int width, int height, int rows, int cols, char * saveTo, std::string rowNames[], std::string colNames[]) {

	double cellWidth = width / cols;
	double cellHeight = height / rows;

	std::vector<SpriteSheetCell> cells;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			SpriteSheetCell cell;
			cell.cellType = colNames[j] + "-of-" + rowNames[i];
			cell.x = j * cellWidth;
			cell.y = i * cellHeight;
			cell.width = cellWidth;
			cell.height = cellHeight;
			cells.push_back(cell);
		}
	}

	SpriteSheetLoader::save(saveTo, cells);
}