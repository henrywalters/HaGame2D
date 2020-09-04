#pragma once

namespace TGM {

	struct LevelCell {
		int id;
		int x;
		int y;
		int cellType;
		bool active;
	};

	struct CellConnection {
		LevelCell* cellA;
		LevelCell* cellB;
	};

	struct LevelData {
		std::vector<LevelCell*> cells;
		std::vector<CellConnection*> connections;
	};

	class LevelLoader {
	public:
		static void save(LevelData data, std::string filePath) {
			std::ofstream of(filePath);

			for (auto cell : data.cells) {
				if (cell->active) {
					of << "T " << cell->id << " " << cell->x << " " << cell->y << " " << cell->cellType << "\n";
				}
			}

			for (auto connection : data.connections) {
				if (connection->cellA->active && connection->cellB->active) {
					of << "C " << connection->cellA->id << " " << connection->cellB->id << "\n";
				}
			}

			of.close();
		}

		static LevelData load(std::string filePath) {
			std::ifstream ifs(filePath);
			LevelData data;
			data.cells = std::vector<LevelCell*>();
			data.connections = std::vector<CellConnection*>();

			std::string line;
			std::vector<std::string> connectionLines = std::vector<std::string>();


			while (std::getline(ifs, line)) {
				auto parts = Util::split(line, ' ');
				if (parts.size() > 0) {
					auto type = parts[0];
					if (type == "T") {
						if (parts.size() != 5) {
							throw new std::exception("T line for level must have 5 parts");
						}

						LevelCell* cell = new LevelCell();
						cell->active = true;
						cell->id = stoi(parts[1]);
						cell->x = stoi(parts[2]);
						cell->y = stoi(parts[3]);
						cell->cellType = stoi(parts[4]);

						data.cells.push_back(cell);
					}
					else if (type == "C") {
						if (parts.size() != 3) {
							throw new std::exception("C line for level must have 3 parts");
						}
						connectionLines.push_back(line);
					}
				}
			}

			for (auto cLine : connectionLines) {
				CellConnection* conn = new CellConnection();
				auto parts = Util::split(cLine, ' ');
				int aId = stoi(parts[1]);
				int bId = stoi(parts[2]);

				for (auto cell : data.cells) {
					if (cell->id == aId) {
						conn->cellA = cell;
					}

					if (cell->id == bId) {
						conn->cellB = cell;
					}
				}

				data.connections.push_back(conn);
			}

			return data;
		}
	};
}