#pragma once
#include "HaGame2D.h"
#include "BinaryGrid.h"
#include "IntervalComponent.h"

const int GRID_X = 40;
const int GRID_Y = 40;

const float TICK_SPEED = 500000;

const RGB SNAKE_COLOR = Color::blue();
const RGB APPLE_COLOR = Color::green();

struct GridPosition {
	int x;
	int y;

	bool operator == (GridPosition pos) {
		return pos.x == x && pos.y == y;
	}

	std::string toString() {
		return "GridPosition<" + std::to_string(x) + "," + std::to_string(y) + ">";
	}
};

class SnakeController : public IntervalComponent {

	structures::DoublyLinkedList<GridPosition> _snake;
	Vector _moveDirection;
	BinaryGrid* _grid = NULL;

public:

	SnakeController() : IntervalComponent(TICK_SPEED) {}

	void placeApple() {
		
		bool validApple = false;
		GridPosition applePos;

		while (!validApple) {
			applePos = GridPosition{
				Random::number(0, _grid->cols()),
				Random::number(0, _grid->rows()),
			};


			validApple = true;
			_snake.iterate([this, applePos, &validApple](GridPosition pos) {
				if (pos == applePos) {
					validApple = false;
				}
			});
		}

		_grid->activate(applePos.x, applePos.y, APPLE_COLOR, "apple");
	}

	void onCreate() {
		_grid = transform->getComponent<BinaryGrid>();
		GridPosition position = GridPosition{
			Random::number(1, _grid->cols() - 1),
			Random::number(1, _grid->rows() - 1)
		};

		int direction = Random::number(0, 4);

		switch (direction) {
		case 0:
			_moveDirection = Vector(1, 0);
			break;
		case 1:
			_moveDirection = Vector(-1, 0);
			break;
		case 2:
			_moveDirection = Vector(0, 1);
			break;
		case 3:
			_moveDirection = Vector(0, -1);
			break;
		}

		_snake.addToFront(position);

		_grid->activate(position.x, position.y, SNAKE_COLOR, "snake");

		increaseSnakeSize();
		// increaseSnakeSize();

		placeApple();
	}

	void increaseSnakeSize() {
		GridPosition back = _snake.back();
		GridPosition nextPos = getNextPosition(back, _moveDirection * -1);
		_snake.addToBack(nextPos);
		_grid->activate(nextPos.x, nextPos.y, SNAKE_COLOR, "snake");
	}

	GridPosition getNextPosition(GridPosition currentPos, Vector moveDirection) {
		int newX = currentPos.x + (int)moveDirection.x;
		int newY = currentPos.y + (int)moveDirection.y;

		if (newX == _grid->cols()) {
			newX = 0;
		}

		if (newX == -1) {
			newX = _grid->cols() - 1;
		}

		if (newY == _grid->rows()) {
			newY = 0;
		}

		if (newY == -1) {
			newY = _grid->rows() - 1;
		}

		GridPosition nextPos = GridPosition{
			newX,
			newY,
		};

		nextPos.x = nextPos.x < 0 ? _grid->cols() - nextPos.x : nextPos.x;
		nextPos.y = nextPos.y < 0 ? _grid->rows() - nextPos.y : nextPos.y;

		return nextPos;
	}

	void updateEveryTick() {
		if (input->left) {
			_moveDirection = Vector(-1, 0);
		}

		if (input->right) {
			_moveDirection = Vector(1, 0);
		}

		if (input->up) {
			_moveDirection = Vector(0, -1);
		}

		if (input->down) {
			_moveDirection = Vector(0, 1);
		}
	}

	void updateEveryInterval(float dt) {

		GridPosition frontPos = _snake.front();
		GridPosition backPos = _snake.removeFromBack();

		_grid->deactivate(backPos.x, backPos.y);

		GridPosition nextPos = getNextPosition(frontPos, _moveDirection);
		log("Front Pos: " + frontPos.toString());
		log("Move Direction: " + _moveDirection.toString());
		log("Deactivated: " + backPos.toString());
		log("Activated: " + nextPos.toString());

		_snake.addToFront(nextPos);
		_grid->activate(nextPos.x, nextPos.y, SNAKE_COLOR, "snake");

		/*
		GridPosition currentPos = _snake.removeFromBack();

		GridPosition nextPos = getNextPosition(currentPos, _moveDirection);

		_grid->iterateNodesWhere("apple", [this, nextPos, currentPos](BinaryGridNode node) {
			if (node.x == nextPos.x && node.y == nextPos.y) {
				placeApple();
			}
		});

		_snake.addToFront(nextPos);

		_grid->deactivate(currentPos.x, currentPos.y);
		
		_grid->activate(nextPos.x, nextPos.y, SNAKE_COLOR, "snake");
		*/
	}
};

class Snake {

	Game* _game;
	Scene* _menu;
	Scene* _snake;

	void prepareMenu() {
		Vector btnSize = Vector(150, 50);
		ButtonFactory btnFactory = ButtonFactory(_menu, RGB{ 0, 0, 0 }, RGB{ 255, 255, 255 }, 36, "../Assets/Fonts/arial.ttf");
		btnFactory.createTextButton(_menu->screenCenter() - (btnSize * 0.5), btnSize, "New Game", [this]() {
			_game->deactivateScene("menu");
			_game->activateScene("snake");
		});
	}

	void prepareSnake() {

		Vector margin = Vector(0, 50);

		GameObject* grid = _snake->add()
			->addComponentAnd(new BinaryGrid(_snake->screenWidth - margin.x, _snake->screenHeight - margin.y, GRID_X, GRID_Y))
			->addComponentAnd(new SnakeController())
			->move(margin);
	}

public:

	Snake() {
		const int width = 600;
		const int height = 600;

		_game = new Game(width, height, "Snake Clone");

		_menu = _game->addScene("menu");
		_snake = _game->addScene("snake", false);

		prepareMenu();
		prepareSnake();
	}

	void run() {
		_game->prepareScene();

		while (_game->running) {

			_game->tick();
		}
	}
};