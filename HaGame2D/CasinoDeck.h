#pragma once
#include "HaGame2D.h"
#include "DeckOfCards.h"

const std::string DECK_TEXTURE = "../Assets/Sprites/HaGameEngine/Misc/cards.png";
const std::string DECK_DEFINITION = "../Assets/SSDefinitions/cards.ssd";

const int CASINO_CARD_WIDTH = 72;
const int CASINO_CARD_HEIGHT = 96;

struct CasinoCard {
	Cards card;
	Suits suit;
	Orientation orientation;
	GameObject *transform;
};

class CasinoDeck : public DeckOfCards<CasinoCard>
{
	Texture texture;
	std::unordered_map<std::string, SpriteSheetCell> deckDef;
	Scene *gameScene;

	Vector origin;

protected:
	CasinoCard defineCard(Suits suitType, Cards cardType);

public:

	GameObject *deckObject;

	CasinoDeck(Scene *scene, Vector position);
	~CasinoDeck();
};

class CasinoCardController : public Component {

	SpriteSheetCell facedown;
	SpriteSheetCell faceup;

	Orientation orientation;

	SpriteRenderer *renderer;

	Vector target;
	bool moving = false;

	double speed = 2;

public:

	CasinoCardController(SpriteSheetCell _facedown, SpriteSheetCell _faceup, Orientation _orientation) {
		facedown = _facedown;
		faceup = _faceup;
		orientation = _orientation;
	};

	void onCreate() {
		renderer = transform->getComponent<SpriteRenderer>();
	}

	void setOrientation(Orientation _orientation) {
		if (_orientation != orientation) {
			orientation = _orientation;
			switch (orientation) {
			case Orientation::Facedown:
				renderer->setSpriteSheetCell(facedown);
				break;
			case Orientation::Faceup:
				renderer->setSpriteSheetCell(faceup);
				break;
			}
		}
	}

	void moveTo(Vector pos) {
		target = pos;
		moving = true;
	}

	void update() {
		if (moving) {
			Vector delta = target - transform->position;

			if (delta.magnitude() > 5) {
				Vector vel = delta.normalized() * speed;
				transform->move(vel);
			}
			else {
				moving = false;
			}
		}
	}
};

class Hand : public Component {
	std::deque<CasinoCard> hand;
	int cardsInHand = 0;
public:
	GameObject *handObject;
	Hand() {}

	void addToHand(CasinoCard card) {
		hand.push_back(card);
		cardsInHand++;
		auto controller = card.transform->getComponent<CasinoCardController>();
		Vector cardPos = Vector(transform->position.x + (cardsInHand * CASINO_CARD_WIDTH * 0.33), transform->position.y + (cardsInHand * CASINO_CARD_HEIGHT * -0.33));
		controller->moveTo(cardPos);
		controller->setOrientation(Orientation::Faceup);
	}

	void addToHandFaceDown(CasinoCard card) {
		hand.push_back(card);
		cardsInHand++;
		auto controller = card.transform->getComponent<CasinoCardController>();
		Vector cardPos = Vector(transform->position.x + (cardsInHand * CASINO_CARD_WIDTH * 0.33), transform->position.y + (cardsInHand * CASINO_CARD_HEIGHT * -0.33));
		controller->moveTo(cardPos);
		controller->setOrientation(Orientation::Facedown);
	}

	void emptyHand(CasinoDeck *deck) {
		while (!hand.empty()) {
			cardsInHand--;
			CasinoCard card = hand.back();
			hand.pop_back();
			deck->discard(card);
			auto controller = card.transform->getComponent<CasinoCardController>();
			controller->moveTo(deck->deckObject->position);
			controller->setOrientation(Orientation::Facedown);
			controller->transform->z_index = 0;
		}
	}

	std::vector<int> calculatePossibleTotalScores() {
		//Sum up all none aces but take note of them as we will 
		int aces = 0;
		int total = 0;

		std::vector<int> totalScores;

		for (int i = 0; i < hand.size(); i++) {
			if (hand[i].card == Cards::Ace) {
				aces++;
			}
			else if (hand[i].card >= 10) {
				total += 10;
			}
			else {
				total += (int)hand[i].card + 1;
			}
		}

		if (aces == 0) {
			totalScores.push_back(total);
		}
		else {
			//Only one ace can be 11 so just add all but last to 1
			for (int i = 0; i < aces - 1; i++) {
				total += 1;
			}

			//1 is obviously an option.
			totalScores.push_back(total + 1);

			if (total + 11 <= 21) {
				totalScores.push_back(total + 11);
			}
		}

		return totalScores;
	}

	int calculateBestScore() {
		auto scores = calculatePossibleTotalScores();
		int best = 0;
		for (int score : scores) {
			if (score > best) {
				best = score;
			}
		}

		return best;
	}

	std::string printScore() {
		auto scores = calculatePossibleTotalScores();

		if (scores.size() == 1) {
			return std::to_string(scores[0]);
		}
		else {
			return std::to_string(scores[0]) + " or " + std::to_string(scores[1]);
		}
	}

	bool hasBusted() {
		return calculateBestScore() > 21;
	}

	bool hasAce() {
		for (int i = 0; i < hand.size(); i++) {
			if (hand[i].card == Cards::Ace) {
				return true;
			}
		}
		return false;
	}

	bool isSplit() {
		return hand.size() == 2 && hand[0].card == hand[1].card;
	}

	bool getSplitValue() {
		return hand[0].card == Cards::Ace ? 11 : hand[0].card >= 10 ? 10 : hand[0].card + 1;
	}

	bool canDoubleDown() {
		return hand.size() == 2;
	}

	std::vector<CasinoCard> getHand() {
		std::vector<CasinoCard> cards;
		for (int i = 0; i < hand.size(); i++) {
			cards.push_back(hand[i]);
		}
		return cards;
	}

	CasinoCard firstFaceupCard() {
		for (int i = 0; i < hand.size(); i++) {
			std::cout << "Checking " << std::to_string(hand[i].card) << "\n";
			if (hand[i].orientation == Orientation::Faceup) {
				return hand[i];
			}
		}
		std::cout << "WARNING - didnt find a face up card\n";
	}
};