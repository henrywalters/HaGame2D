#pragma once
#include "Strategy.h"
#include "CasinoDeck.h"

/*
Refer to this https://wizardofodds.com/games/blackjack/strategy/4-decks/ for the weird constants used in context grids.
*/

//equates to 2, 3, 4, 5, 6, 7, 8, 9, 10 & A
const std::vector<Cards> DEALER_HANDS {
	(Cards) 1,
	(Cards) 2,
	(Cards) 3,
	(Cards) 4,
	(Cards) 5,
	(Cards) 6,
	(Cards) 7,
	(Cards) 8,
	(Cards) 9,
	(Cards) 0
};

const std::vector<std::string> BASIC_MOVES{
	"H", "S", "Dh", "Ds", "P", "Ph", "Rh"
};

enum BlackjackMoves {
	Hit,
	Stick,
	Split,
	DoubleDown
};

enum BlackjackStrategies {
	Dealer
};

struct PlayerContext {
	Hand *hand;
	CasinoCard dealerCard;
};

struct ContextGrid {
	std::vector<int> handValues;
	std::vector<std::vector<int>> grid;
};

static int gridCell(ContextGrid context, int value, Cards card) {
	int rowIndex, colIndex;

	//Any valid context grid will always succeed

	for (int i = 0; i < DEALER_HANDS.size(); i++) {
		if (DEALER_HANDS[i] == card) {
			colIndex = i;
			break;
		}
	}

	for (int i = 0; i < context.handValues.size(); i++) {
		if (context.handValues[i] == value) {
			rowIndex = i;
			break;
		}
	}
	std::cout << "grid row: " << rowIndex << " col: " << colIndex << "\n";
	if (rowIndex < 0 || rowIndex >= context.grid.size()) {
		throw new std::exception("Invalid row index " + rowIndex);
	}
	else if (colIndex < 0 || colIndex >= context.grid[rowIndex].size()) {
		throw new std::exception("Invalid col index " + colIndex);
	}
	return context.grid[rowIndex][colIndex];
}

struct InvalidContextGridException : public std::exception {
	std::string message_;
public:
	InvalidContextGridException(const std::string &message) : message_(message) {

	}

	const char *what() const throw() {
		return ("Invalid context grid with message: " + message_).c_str();
	}
};

static bool validContextGrid(ContextGrid context) {
	int rowSize = DEALER_HANDS.size();

	if (context.handValues.size() != context.grid.size()) {
		std::cout << "ERROR - Context grid does not have the right amount of rows\n";
		return false;
	}

	for (auto row : context.grid) {
		if (row.size() != rowSize) {
			std::cout << "ERROR - Context grid has row which is not the proper size\n";
			return false;
		}

		for (auto choice : row) {
			if (choice < 0 || choice >= BASIC_MOVES.size()) {
				std::cout << "ERROR - Context grid has cell which is not a valid basic move\n";
				return false;
			}
		}
	}

	return true;
}

const ContextGrid HARD_HAND_GRID {
	{ 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 }, 
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 2, 2, 2, 2, 0, 0, 0, 0, 0 },
		{ 0, 2, 2, 2, 2, 2, 2, 2, 0, 0 },
		{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 6, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 6, 6, 6 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	}
};

const ContextGrid SOFT_HAND_GRID{
	{ 13, 14, 15, 16, 17, 18, 19, 20, 21 },
	{
		{ 0, 0, 0, 2, 2, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 2, 2, 0, 0, 0, 0, 0 },
		{ 0, 0, 2, 2, 2, 0, 0, 0, 0, 0 },
		{ 0, 0, 2, 2, 2, 0, 0, 0, 0, 0 },
		{ 0, 2, 2, 2, 2, 0, 0, 0, 0, 0 },
		{ 1, 3, 3, 3, 3, 1, 1, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	}
};

//note here that this is just one card as the value!! ie (2, 2), (3, 3)... (A, A)
const ContextGrid SPLIT_HAND_GRID{
	{ 2, 3, 4, 6, 7, 8, 9, 11 },
	{
		{ 5, 5, 4, 4, 4, 4, 0, 0, 0, 0 },
		{ 5, 5, 4, 4, 4, 4, 0, 0, 0, 0 },
		{ 0, 0, 0, 5, 5, 0, 0, 0, 0, 0 },
		{ 5, 4, 4, 4, 4, 0, 0, 0, 0, 0 },
		{ 4, 4, 4, 4, 4, 4, 0, 0, 0, 0 },
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
		{ 4, 4, 4, 4, 4, 1, 4, 4, 1, 1 },
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	}
};

class DealerStrategy : public Strategy<int, BlackjackMoves> {

	int highCard = 17;

public:
	DealerStrategy();
	~DealerStrategy();

	BlackjackMoves evaluate(int handTotal);

};

class BasicStrategy : public Strategy<PlayerContext, BlackjackMoves> {

public:
	BasicStrategy();
	~BasicStrategy();

	BlackjackMoves evaluate(PlayerContext context);

};
