#pragma once
#include "HaGame2D.h"
#include "TGMLevelLoader.h"

namespace TGM {

	const std::string saveTo = "Assets/Levels/level3.txt";

	std::vector<CellConnection*> connections = std::vector<CellConnection*>();

	const Vector GRID_SIZE = Vector(1500, 1500);
	const Vector GRID_PARTITIONS = Vector(100, 100);
	const Vector CELL_SIZE = Vector(GRID_SIZE.x / GRID_PARTITIONS.x, GRID_SIZE.y / GRID_PARTITIONS.y);

	const float CAMERA_SLOW = 100.0f;
	const float CAMERA_FAST = 200.0f;

	const float CAMERA_ZOOM_SLOW = 0.1f;
	const float CAMERA_ZOOM_FAST = 0.5f;

	int* toolkitOption = new int(0);
	int* tool = new int(0);

	LevelCell* selectedCell;

	Grid<LevelCell*>* cells = new Grid<LevelCell*>(GRID_SIZE.x, GRID_SIZE.y, GRID_PARTITIONS.x, GRID_PARTITIONS.y);

	enum TOOL_ENUM {
		DRAWER,
		CONNECTOR,
	};

	const std::vector<std::string> TOOLS = {
		"DRAWER",
		"CONNECTOR",
	};

	enum TOOLKIT_ENUM {
		WALL,
		LEVER,
		DOOR,
		START,
		FINISH,
		HEALTH,
		ENEMY
	};

	const std::vector<std::string> TOOLKIT_OPTIONS = {
			"WALL",
			"LEVER",
			"DOOR",
			"START",
			"FINISH",
			"HEALTH",
			"ENEMY",
	};

	const std::vector<Color> TOOLKIT_COLORS = {
		FG_COLOR,
		BLUE,
		GREEN,
		LIGHT_BLUE,
		ORANGE,
		WHITE,
		RED,
	};

	enum BuilderAction {
		Draw,
		Erase,
	};

	void clearCells() {
		for (int i = 0; i < cells->rows; i++) {
			for (int j = 0; j < cells->cols; j++) {
				LevelCell* cell = new LevelCell();
				cell->id = j * cells->cols + i;
				cell->active = false;
				cell->cellType = -1;
				cell->x = i;
				cell->y = j;

				cells->set(GridIndex{ i,j }, cell);
			}
		}
	}

	class CameraSystem : public System {

		Scene* scene;
		float speed;
		float zoomSpeed;

	public:
		CameraSystem() : System("Camera System") {}

		void onInit() {
			scene = getScene();

			scene->camera.positionCenter(Vector(GRID_SIZE.x / 2, GRID_SIZE.y / 2));
		}

		void update() {

			float dt = scene->dt_s();

			speed = scene->input->shift ? CAMERA_FAST : CAMERA_SLOW;
			zoomSpeed = scene->input->shift ? CAMERA_ZOOM_FAST : CAMERA_ZOOM_SLOW;

			if (scene->input->up) {
				scene->camera.move(Vector(0, -speed * dt));
			}

			if (scene->input->down) {
				scene->camera.move(Vector(0, speed * dt));
			}

			if (scene->input->left) {
				scene->camera.move(Vector(-speed * dt, 0));
			}

			if (scene->input->right) {
				scene->camera.move(Vector(speed * dt, 0));
			}

			if (scene->input->one) {
				scene->camera.zoomIn(zoomSpeed * dt);
			}

			if (scene->input->two) {
				scene->camera.zoomIn(-zoomSpeed * dt);
			}
			
		}
	};

	class ToolkitSystem : public System {
		Scene* scene;

		Vector rowSize;

		std::unordered_map<std::string, GameObject*> toolOptions;
		std::unordered_map <std::string, GameObject*> options;

	public:

		ToolkitSystem() : System("Toolkit_System") {

		}

		void onInit() {
			scene = getScene();
			int index = 0;
			int toolIndex = 0;
			int optionIndex = 0;
			rowSize = Vector(scene->viewport.get(2), 50);

			addLabel(Vector(0, index * 50), rowSize, 24, TGM::RED, "Tools");
			index++;
		
			for (auto toolOption : TOOLS) {
				auto label = addLabel(Vector(0, index * 50), rowSize, 16, WHITE, toolOption);
				auto btn = label->addComponent(new ButtonComponent(rowSize.x, rowSize.y));
				toolOptions[toolOption] = label;

				btn->onClickFunc = [toolIndex]() {
					tool = new int(toolIndex);
				};
				toolIndex++;
				index++;
			}

			addLabel(Vector(0, index * 50), rowSize, 24, TGM::RED, "Materials");
			index++;

			for (auto option : TOOLKIT_OPTIONS) {
				auto label = addLabel(Vector(0, index * 50), rowSize, 16, TGM::WHITE, option);
				auto btn = label->addComponent(new ButtonComponent(rowSize.x, rowSize.y));
				options[option] = label;

				btn->onClickFunc = [optionIndex]() {
					toolkitOption = new int(optionIndex);
				};

				optionIndex++;
				index++;
			}

			addLabel(Vector(0, index * 50), rowSize, 24, TGM::RED, "Management");
			index++;

			auto saveBtnLabel = addLabel(Vector(0, index * 50), rowSize, 16, TGM::WHITE, "Save");
			auto saveBtn = saveBtnLabel->addComponent(new ButtonComponent(rowSize.x, rowSize.y));
			saveBtn->onClickFunc = []() {
				LevelData data;
				data.cells = cells->flatten();
				data.connections = connections;
				LevelLoader::save(data, saveTo);
			};
			index++;

			auto loadBtnLabel = addLabel(Vector(0, index * 50), rowSize, 16, TGM::WHITE, "Load");
			auto loadBtn = loadBtnLabel->addComponent(new ButtonComponent(rowSize.x, rowSize.y));
			loadBtn->onClickFunc = []() {
				LevelData data = LevelLoader::load(saveTo);
				clearCells();
				connections = data.connections;
				for (auto cell : data.cells) {
					cells->set(cell->x, cell->y, cell);
				}
			};

			index++;

			auto clearBtnLabel = addLabel(Vector(0, index * 50), rowSize, 16, WHITE, "Clear");
			auto clearBtn = clearBtnLabel->addComponent(new ButtonComponent(rowSize.x, rowSize.y));
			clearBtn->onClickFunc = []() {
				clearCells();
				connections.clear();
			};

		}

		void update() {

			for (auto option : TOOLS) {
				auto text = toolOptions[option]->getComponent<TextRenderer>();
				text->setFontColor(tool != NULL & TOOLS[*tool] == option ? BLUE.rgb : WHITE.rgb);
			}

			for (auto option : TOOLKIT_OPTIONS) {
				auto text = options[option]->getComponent<TextRenderer>();
				text->setFontColor(toolkitOption != NULL && TOOLKIT_OPTIONS[*toolkitOption] == option ? BLUE.rgb : WHITE.rgb);
			}
		}

		GameObject* addLabel(Vector pos, Vector size, int fontSize, Color color, std::string msg) {
			auto text = scene->add();
			text->move(pos);
			auto renderer = text->addComponent(new TextRenderer(size.x, size.y));
			renderer->transform->z_index = 15;
			renderer->setAllignment(TextAllignments::Center);
			renderer->setFontSize(fontSize);
			renderer->setFontColor(color.rgb);
			renderer->setMessage(msg);
			return text;
		}
	};

	class DrawingSystem : public System {
	private:
		GridLines* gridLines;
		EventManager<GridIndex>* gridEvents;
		Scene* scene;
		Vector mousePos;
		Vector index;

		Color previewColor;

	public:
		DrawingSystem(GridLines* _gridLines, EventManager<GridIndex>* _gridEvents) : System("Drawing System") {
			gridLines = _gridLines;
			gridEvents = _gridEvents;
		}

		void onInit() {
			scene = getScene();
		}

		void update() {
			auto scale = gridLines->getRelativeScale();
			auto size = Vector(CELL_SIZE.x * scale.x, CELL_SIZE.y * scale.y);
			mousePos = scene->input->globalMousePos();
			if (scene->display->inDisplayPort(scene->input->mousePos())) {
				index = Vector(
					floor((mousePos.x - gridLines->transform->position.x) / CELL_SIZE.x),
					floor((mousePos.y - gridLines->transform->position.y) / CELL_SIZE.y)
				);

				hover();

				if (scene->input->fire1) {

					if (*tool == TOOL_ENUM::DRAWER) {
						gridEvents->emit("draw", GridIndex{ (int)index.x, (int)index.y });
					}

					
					
				}

				if (scene->input->fire2) {
					if (*tool == TOOL_ENUM::DRAWER) {
						gridEvents->emit("erase", GridIndex{ (int)index.x, (int)index.y });
					}
				}

				if (scene->input->fire1Down) {
					if (*tool == TOOL_ENUM::CONNECTOR) {
						if (selectedCell == NULL) {
							selectedCell = cells->get(index.x, index.y);
							// std::cout << selectedCell->id << std::endl;
						}
						else {
							auto connection = new CellConnection();
							connection->cellA = selectedCell;
							connection->cellB = cells->get(index.x, index.y);
							connections.push_back(connection);
							selectedCell = NULL;
						}
					}
				}
				
				if (*tool == TOOL_ENUM::CONNECTOR && selectedCell != NULL) {
					scene->display->drawLine(
						gridLines->transform->relativePosition.x + selectedCell->x * size.x + size.x / 2, 
						gridLines->transform->relativePosition.y + selectedCell->y * size.y + size.y / 2, 
						gridLines->transform->relativePosition.x + index.x * size.x + size.x / 2,
						gridLines->transform->relativePosition.y + index.y * size.y + size.y / 2,
						RED.rgb, 
						20);
				}
				
			}

			for (int i = 0; i < cells->rows; i++) {
				for (int j = 0; j < cells->cols; j++) {
					LevelCell* cell = nullptr;
					cell = cells->get(i, j);
					if (cell->active) {
						scene->display->fillRect(
							gridLines->transform->relativePosition.x + i * size.x,
							gridLines->transform->relativePosition.y + j * size.y,
							size.x,
							size.y,
							TOOLKIT_COLORS[cell->cellType].rgb
						);
					}
				}
			}

			for (auto connection : connections) {
				scene->display->drawLine(
					gridLines->transform->relativePosition.x + connection->cellA->x * size.x + size.x / 2,
					gridLines->transform->relativePosition.y + connection->cellA->y * size.y + size.y / 2,
					gridLines->transform->relativePosition.x + connection->cellB->x * size.x + size.x / 2,
					gridLines->transform->relativePosition.y + connection->cellB->y * size.y + size.y / 2,
					RED.rgb,
					20
				);
			}
		}

		void hover() {

			previewColor = TOOLKIT_COLORS[*toolkitOption];

			auto scale = gridLines->getRelativeScale();
			auto size = Vector(CELL_SIZE.x * scale.x, CELL_SIZE.y * scale.y);
			auto pos = Vector(1 + gridLines->transform->relativePosition.x + size.x * index.x, 1 + gridLines->transform->relativePosition.y + size.y * index.y);

			if (*tool == TOOL_ENUM::DRAWER) {
				scene->display->fillRect(pos.x, pos.y, size.x, size.y, previewColor.rgb);
			}
			else {
				scene->display->drawRect(pos.x - 5, pos.y - 5, size.x + 10, size.y + 10, BLUE.rgb);
			}
			
		}
	};

	

	class LevelBuilder : public Game {

		Scene* builder;
		Scene* toolkit;

		EventManager<GridIndex> *gridEvents;

	public:
		LevelBuilder() : Game(1000, 800, "The Great Machine: Level Builder") {

			cells->initialize(new LevelCell{ });

			clearCells();

			gridEvents = new EventManager<GridIndex>();

			gridEvents->on("draw", [this](GridIndex v) {
				LevelCell* cell = nullptr;
				cell = cells->get(v);
				cell->active = true;
				cell->cellType = *toolkitOption;

				cells->set(v, cell);
			});

			gridEvents->on("erase", [](GridIndex v) {
				LevelCell* cell = nullptr;
				cell = cells->get(v);
				cell->active = false;
				cells->set(v, cell);
			});

			builder = addScene("builder");
			toolkit = addScene("toolkit");

			builder->setDisplayPort(0, 0, 750, 800);
			toolkit->setDisplayPort(750, 0, 250, 800);

			builder->camera.position(Vector(GRID_SIZE.x / 2, GRID_SIZE.y / 2));

			auto grid = builder->add()
				->addComponentAnd(new GridLines(GRID_SIZE.x, GRID_SIZE.y, GRID_PARTITIONS.x, GRID_PARTITIONS.y));

			grid->getComponent<GridLines>()->color = Color(0x00, 0x00, 0xFF, 0x7D).rgb;

			builder->addSystem(new CameraSystem());

			toolkitOption = new int(0);

			builder->addSystem(new DrawingSystem(grid->getComponent<GridLines>(), gridEvents));

			toolkit->addSystem(new ToolkitSystem());
		}

		void run() {
			builder->initialize();
			toolkit->initialize();

			while (running) {
				tick();
			}
		}
	};
}