#pragma once
#include <deque>
#include <vector>
#include <exception>

enum Suits {
	Clubs,
	Spades,
	Hearts,
	Diamonds
};

enum Cards {
	Ace,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King
};

enum Orientation {
	Faceup,
	Facedown
};

const std::string SUITS[] = {
	"Clubs",
	"Spades",
	"Hearts",
	"Diamonds"
};

const std::string CARDS[] = {
	"Ace",
	"Two",
	"Three",
	"Four",
	"Five",
	"Six",
	"Seven",
	"Eight",
	"Nine",
	"Ten",
	"Jack",
	"Queen",
	"King"
};

struct OutOfCardsException : public std::exception {
	const char *what() const throw() {
		return "Attempted to pull a card out of an empty stack";
	}
};

struct InvalidDeckException : public std::exception {
	const char *what() const throw() {
		return "Invalid deck created. Expected 52 cards for each deck.";
	}
};

const int DEFAULT_DECK_COUNT = 12;

template <class T>
class DeckOfCards
{

	bool initialized = false;

protected:

	std::vector<T> allCards;
	std::deque<T> deck;
	std::deque<T> discardPile;
	int deckCount;
	int allCardsTotal;


private:

	void initializeDeck() {
		allCardsTotal = 52 * deckCount;
		for (int i = 0; i < deckCount; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 13; k++) {
					allCards.push_back(defineCard((Suits) j, (Cards) k));
					std::cout << allCards.size() << "\n";
				}
			}
		}
	}

protected:

	virtual T defineCard(Suits suit, Cards card) = 0;

public:

	DeckOfCards() {
		deckCount = DEFAULT_DECK_COUNT;
	}

	DeckOfCards(int _deckCount) {
		deckCount = _deckCount;
	}

	virtual ~DeckOfCards() {}

	//Reset the deck to a stack of cards ordered by suit then card type and the discard pile to an empty stack of cards
	void reset() {

		if (!initialized) {
			initializeDeck();
			initialized = true;
			std::cout << "All Cards Total: " << allCardsTotal << "\n";
		}

		discardPile.clear();
		deck.clear();

		std::cout << "All Cards Total: " << allCardsTotal << "\n";

		for (int i = 0; i < allCardsTotal; i++) {
			addToTop(allCards[i]);
		}
	}

	//Shuffle all cards in the deck
	void shuffle() {
		srand(time(0));
		for (int i = 0; i < deck.size(); i++) {
			std::random_shuffle(deck.begin(), deck.end());
		}
	}

	//Remove card from top of deck
	T deal() {
		if (!deck.empty()) {
			auto card = deck.back();
			deck.pop_back();
			return card;
		}
		else {
			throw new OutOfCardsException();
		}
	}

	//Returns, but does not remove, the top card from the deck
	T peak() {
		if (!deck.empty()) {
			auto card = deck.back();
			return card;
		}
		else {
			throw new OutOfCardsException();
		}
	}

	//Returns, but does not remove, a card from an index
	T peak(int index) {
		if (index <= deck.size() && index >= 0) {
			return deck[index];
		}
		else {
			throw new OutOfCardsException();
		}
	}

	//Add a card to the top of the discard pile
	void discard(T card) {
		discardPile.push_back(card);
	}

	//Remove card from top of discard pile
	T takeFromDiscard(T card) {
		if (!discardPile.empty()) {
			auto card = discardPile.back();
			discardPile.pop_back();
			return card;
		}
		else {
			throw new OutOfCardsException();
		}
	}

	//Add a card to the bottom of the deck
	void addToBottom(T card) {
		deck.push_front(card);
	}

	//Add a card to the top of the deck
	void addToTop(T card) {
		deck.push_back(card);
	}

	int size() {
		return deck.size();
	}

	bool empty() {
		return deck.size() == 0;
	}
};

static std::string cardTag(int suitIndex, int cardIndex) {
	return CARDS[cardIndex] + "-of-" + SUITS[suitIndex];
}

static std::string cardName(int suitIndex, int cardIndex) {
	return CARDS[cardIndex] + " of " + SUITS[suitIndex];
}
