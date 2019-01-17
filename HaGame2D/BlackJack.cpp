#include "stdafx.h"
#include "BlackJack.h"
#include <stack>
#include "ButtonMenu.h"
#include <algorithm>

const int CARD_WIDTH = 72;
const int CARD_HEIGHT = 96;

const int WIDTH = 1800;
const int HEIGHT = 800;

const std::string DECK_TEXTURE = "../Assets/Sprites/HaGameEngine/Misc/cards.png";
const std::string DECK_DEFINITION = "../Assets/SSDefinitions/cards.ssd";

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

std::string cardTag(int suitIndex, int cardIndex) {
	return CARDS[cardIndex] + "-of-" + SUITS[suitIndex];
}

class CardController : public Component {

	SpriteSheetCell facedown;
	SpriteSheetCell faceup;

	Orientation orientation;

	SpriteRenderer *renderer;

	Vector target;
	bool moving = false;

	float speed = 3;

public:

	CardController(SpriteSheetCell _facedown, SpriteSheetCell _faceup, Orientation _orientation) {
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
			Vector delta = transform->position - target;
			
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

struct Card {

	Cards card;
	Suits suit;
	Orientation orientation;
	GameObject *transform;

	std::string tag() {
		return cardTag(suit, card);
	}
};

const int DECKS = 8;

class Deck {
	std::stack<Card> deck;
	int decks;

	Texture texture;
	std::unordered_map<std::string, SpriteSheetCell> deckDef;
	Scene *gameScene;

public:

	GameObject *deckObject;

	Deck(Scene *scene) {
		gameScene = scene;
		int index = 0;

		deckObject = scene->add();

		texture = scene->display->loadTexture(DECK_TEXTURE);
		deckDef = SpriteSheetLoader::getSpriteMap(DECK_DEFINITION);

		for (int k = 0; k < DECKS; k++) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 13; j++) {
					Card card;
					card.card = (Cards)j;
					card.suit = (Suits)i;
					card.orientation = Orientation::Facedown;
					card.transform = scene->add();
					card.transform->addComponent(new SpriteRenderer(texture, deckDef["Facedown"], CARD_WIDTH, CARD_HEIGHT));
					card.transform->addComponent(new CardController(deckDef["Facedown"], deckDef[card.tag()], card.orientation));
					card.transform->move(Vector(index * 0.5 + 150, index * -0.5 + 150));
					index++;
					deck.push(card);
				}
			}
		}
	}

	void shuffle() {
		
		std::stack<Card> tmpDeck;
		std::vector<Card> cards;

		while (!deck.empty()) {
			cards.push_back(deck.top());
			deck.pop();
		}

		std::random_shuffle(cards.begin(), cards.end());

		for (int i = 0; i < cards.size(); i++) {
			tmpDeck.push(cards[i]);
		}

		deck.swap(tmpDeck);
	}

	Card deal() {
		if (!deck.empty()) {
			auto card = deck.top();
			deck.pop();
			return card;
		}
		else {
			Card card;

			card.transform = gameScene->add();

			card.transform->addComponent(new SpriteRenderer(texture, deckDef["Joker"], CARD_WIDTH, CARD_HEIGHT));
			card.transform->addComponent(new CardController(deckDef["Facedown"], deckDef["Joker"], Orientation::Faceup));

			return card;
		}
	}

	bool canDeal() {
		return !deck.empty();
	}

	void addToDeck(Card card) {
		deck.push(card);
	}
};

class Hand : public Component {
	std::stack<Card> hand;

public:
	GameObject *handObject;
	Hand(Scene *scene) {
		handObject = scene->add();
	}

	void addToHand(Card card) {
		hand.push(card);
		auto controller = card.transform->getComponent<CardController>();
		controller->moveTo(handObject->position);
		controller->setOrientation(Orientation::Faceup);
	}

	void emptyHand(Deck *deck) {
		while (!hand.empty()) {
			Card card = hand.top();
			deck->addToDeck(card);
		}
	}
};



class RandomCardMover : public Component {
public:

	const float speed = 3;
	Vector velocity;

	const float epsilon = 5;

	RandomCardMover() {};

	void onCreate() {
		velocity = Vector(Random::numberPlusMinus(1, 4), Random::numberPlusMinus(1, 4));

		velocity.normalize();
		
		velocity = velocity * speed;
	}

	void update() {

		auto box = transform->getComponent<BoxComponent>();

		if (transform->position.x <= epsilon || transform->position.x + box->width >= WIDTH - epsilon) {
			velocity.x *= -1;
		}

		if (transform->position.y <= epsilon || transform->position.y + box->height >= HEIGHT - epsilon) {
			velocity.y *= -1;
		}

		transform->move(velocity);
	}
};

BlackJack::BlackJack()
{
	Game *blackjack = new Game(WIDTH, HEIGHT, "Black Jack");
	Scene *board = blackjack->addScene("board");

	MenuItem<std::string> m1, m2, m3, m4;
	
	board->setDisplayPort(0, 0, WIDTH, HEIGHT);

	auto bg = board->add();
	bg->addComponent(new BoxRenderer(WIDTH, HEIGHT, true, Color(47, 132, 30).rgb));
	bg->z_index = 0;

	Deck deck = Deck(board);
	Hand hand = Hand(board);
	
	auto menu = board->add();

	Style style;

	m1.label = "Single Player"; m2.label = "Two Players"; m3.label = "Three Players"; m4.label = "Four Players";
	m1.value = 1; m2.value = 2; m3.value = 3; m4.value = 4;

	std::vector<MenuItem<std::string>> options = {
		m1,
		m2,
		m3,
		m4
	};

	menu->addComponent(new ButtonMenu<std::string>(options));

	deck.deckObject->move(Vector(600, 600));
	hand.handObject->move(Vector(200, -400));

	//deck.shuffle();

	blackjack->prepareScene();

	while (blackjack->running && deck.canDeal()) {
		blackjack->tick();
	}
}

BlackJack::~BlackJack()
{
}