#include "stdafx.h"
#include "BlackjackStrategy.h"

DealerStrategy::DealerStrategy()
{
}

DealerStrategy::~DealerStrategy()
{
}

BlackjackMoves DealerStrategy::evaluate(int handTotal)
{
	return (handTotal >= highCard) ? BlackjackMoves::Stick : BlackjackMoves::Hit;
}

BasicStrategy::BasicStrategy()
{
	if (!validContextGrid(HARD_HAND_GRID)) {
		throw new InvalidContextGridException("Hard Hand");
	}
	else if (!validContextGrid(SOFT_HAND_GRID)) {
		throw new InvalidContextGridException("Soft Hand");
	}
	else if (!validContextGrid(SPLIT_HAND_GRID)) {
		throw new InvalidContextGridException("Split Hand");
	}
}

BasicStrategy::~BasicStrategy()
{
}

BlackjackMoves BasicStrategy::evaluate(PlayerContext context)
{
	std::string move;
	int moveIndex;

	if (context.hand->isSplit()) {
		moveIndex = gridCell(SPLIT_HAND_GRID, context.hand->getSplitValue(), context.dealerCard.card);
	}
	else {
		if (context.hand->hasAce()) {
			moveIndex = gridCell(SOFT_HAND_GRID, context.hand->calculateBestScore(), context.dealerCard.card);
		}
		else {
			moveIndex = gridCell(HARD_HAND_GRID, context.hand->calculateBestScore(), context.dealerCard.card);
		}
	}

	move = BASIC_MOVES[moveIndex];

	if (move == "H" || move == "Rh" || move == "Ph") {
		return BlackjackMoves::Hit;
	}
	else if (move == "S") {
		return BlackjackMoves::Stick;
	}
	else if (move == "Dh") {
		return context.hand->canDoubleDown() ? BlackjackMoves::DoubleDown : BlackjackMoves::Hit;
	}
	else if (move == "Ds") {
		return context.hand->canDoubleDown() ? BlackjackMoves::DoubleDown : BlackjackMoves::Stick;
	}
	else if ("P") {
		return BlackjackMoves::Split;
	}
	else {
		std::cout << "WARNING - invalid basic strategy move. By default, just sticking.";
		return BlackjackMoves::Stick;
	}
}
