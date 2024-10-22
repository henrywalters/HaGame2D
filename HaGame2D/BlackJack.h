#pragma once
#include "HaGame2D.h"
#include "CasinoDeck.h"
#include "CasinoGame.h"
#include "BlackjackStrategy.h"
#include "Loop.h"
class Timeout : public Component {

	std::function<void()> _callback;
	int start;
	int _duration;

public:
	Timeout(int duration, std::function<void()> callback) {
		_callback = callback;
		_duration = duration;
		std::cout << "Created " << duration << " ms timeout \n";
	}

	void onCreate() {
		start = SDL_GetTicks();
		std::cout << "Timing out in " + std::to_string(_duration) + " milliseconds\n";
	}

	void update() {

		int elapsedTime = SDL_GetTicks() - start;

		if (elapsedTime >= _duration) {
			gameScene->destroy(transform);
			std::cout << "Timed out!\n";
			_callback();
		}
		else {
			//std::cout << "Time left: " << std::to_string(_duration - elapsedTime) << "\n";
		}
	}
};

class BlackJack : public CasinoGame<void>
{
	bool initialized = false;
	CasinoDeck *deck;
	GameObject *blackjack;

	Vector dealerOrigin;

	std::vector<Vector> playerOrigins;
	std::vector<Hand *> playerHands;
	std::vector<GameObject *> playerPromptBtns;
	Hand * dealerHand;
	CasinoCard dealerFaceupCard;

public:
	BlackJack() : CasinoGame<void>("BlackJack") {
	}

	~BlackJack() {};

	void deal() {
		//2 cards for every player and the dealer
		int bjPlayers = participants.size() + 1;
		int cardsToDeal = 2 * bjPlayers;

		for (int i = 0; i < cardsToDeal; i++) {
			int bjPlayerIndex = i % bjPlayers;

			if (bjPlayerIndex == bjPlayers - 1) {
				if (i == bjPlayers - 1) {
					dealerHand->addToHandFaceDown(deck->deal());
				}
				else {
					dealerFaceupCard = deck->deal();
					dealerHand->addToHand(dealerFaceupCard);
				}
			}
			else {
				playerHands[bjPlayerIndex]->addToHand(deck->deal());
			}
		}
	}

	void promptDealer() {

	}

	void killPromptBtns() {
		for (auto btn : playerPromptBtns) {
			scene->destroy(btn);
		}

		playerPromptBtns.clear();
	}

	void promptPlayer(int playerIndex) {
		auto prompt = new GameObject();

		auto btnFactory = ButtonFactory(scene, Color::white(), Color::black(), 24, "../Assets/Fonts/Casino3D.ttf");
		auto labelFactory = TextLabelFactory(scene, "../Assets/Fonts/Casino3D.ttf", 24, Color::black());

		auto origin = playerOrigins[playerIndex];

		GameObject * scoreLabel;
		scoreLabel = labelFactory.build(playerHands[playerIndex]->printScore(), origin + Vector(-15, 75), Vector(60, 30));

		GameObject * hitBtn = btnFactory.createTextButton(origin + Vector(45, 150), Vector(60, 40), "Hit", [this, playerIndex, &labelFactory, &origin, &scoreLabel]() {
			playerHands[playerIndex]->addToHand(deck->deal());
			
			if (scoreLabel != NULL) {
				scene->destroy(scoreLabel);
			}

			scoreLabel = labelFactory.build(playerHands[playerIndex]->printScore(), origin + Vector(-15, 75), Vector(60, 30));

			std::cout << "Player: " << playerIndex << " score is: " << playerHands[playerIndex]->printScore() << "\n";

			if (playerHands[playerIndex]->hasBusted()) {
				killPromptBtns();

				std::cout << "Player: " << playerIndex << " busted\n";
				if (playerIndex > 0) {
					promptPlayer(playerIndex - 1);
				}
				else {
					promptDealer();
				}
			}
			else {
				if (playerHands[playerIndex]->calculateBestScore() == 21) {
					killPromptBtns();
					if (playerIndex > 0) {
						promptPlayer(playerIndex - 1);
					}
					else {
						promptDealer();
					}
				}
			}
		});
		GameObject * dummyObject = prompt->add();
		dummyObject = btnFactory.createTextButton(origin + Vector(-35, 150), Vector(60, 40), "Hold", [this, playerIndex, hitBtn, dummyObject, prompt]() {

			scene->destroy(prompt);

			killPromptBtns();

			if (playerIndex > 0) {
				promptPlayer(playerIndex - 1);
			}
			else {
				
				promptDealer();
			}
		});

		playerPromptBtns.push_back(dummyObject);
		playerPromptBtns.push_back(hitBtn);
		
	}

	void play(Resolution<void> resolution) {

		std::cout << "Playing blackjack\n";

		if (!initialized) {

			Vector deckOrigin = Vector(scene->screenWidth / 2, CASINO_CARD_HEIGHT);

			blackjack = new GameObject();
			deck = new CasinoDeck(scene, deckOrigin + Vector(200, 0));
			auto bg = new GameObject();
			bg->addComponent(new BoxRenderer(1000, 1000, true, Color(32, 73, 25).rgb));
			bg->z_index = 1;

			dealerOrigin = Vector(scene->screenWidth / 2 - CASINO_CARD_WIDTH / 2, 150);

			deck->deckObject->move(deckOrigin);

			auto fg = new GameObject();
			
			double spacing = Math::getUniformDistrubtionSpacing(scene->screenWidth, CASINO_CARD_WIDTH, participants.size());

			auto dealerHandObject = new GameObject();
			dealerHandObject->move(dealerOrigin);
			dealerHand = dealerHandObject->addComponent(new Hand());
			scene->instantiate(dealerHandObject);

			for (int i = 0; i < participants.size(); i++) {
				Vector playerOrigin = Vector(Math::mapToUniformDistribution(CASINO_CARD_WIDTH, spacing, i), scene->screenHeight - 3 * CASINO_CARD_HEIGHT);
				auto hand = new GameObject();
				hand->move(playerOrigin);
				auto handController = hand->addComponent(new Hand());
				playerHands.push_back(handController);
				scene->instantiate(hand);
				playerOrigins.push_back(playerOrigin);
				auto fgPiece = fg->add()->move(playerOrigin);
				fgPiece->addComponent(new BoxRenderer(CASINO_CARD_WIDTH, CASINO_CARD_HEIGHT, false, Color(220, 220, 220).rgb));
				scene->instantiate(fgPiece);
			}
			

			scene->instantiate(blackjack);
			scene->instantiate(bg);

			initialized = true;
		}

		deck->reset();
		deck->shuffle();

		for (int i = 0; i < deck->size(); i++) {
			deck->peak(i).transform->z_index = deck->size() - i;
		}

		int players = participants.size();
		int dealt = 0;

		deal();

		

		Loop::For(scene, 0, deck->size(), 1, 1500, [this](int index) {

			index = index % (participants.size() + 2);

			if (index < participants.size()) {
				BasicStrategy strategy;
				BlackjackMoves move = BlackjackMoves::Hit;
				bool doubleDowned = false;
				while (move != BlackjackMoves::Stick && !doubleDowned && playerHands[index]->calculateBestScore() < 21) {
					PlayerContext context;
					context.dealerCard = dealerFaceupCard;
					context.hand = playerHands[index];

					move = strategy.evaluate(context);

					if (move == BlackjackMoves::Hit || move == BlackjackMoves::Split) {
						context.hand->addToHand(deck->deal());
					}
					else if (move == BlackjackMoves::DoubleDown) {
						context.hand->addToHand(deck->deal());
					}
				}

			}
			else if (index == participants.size()) {
				DealerStrategy dStrat;
				BlackjackMoves dMove = BlackjackMoves::Hit;
				while (dMove != BlackjackMoves::Stick) {
					dMove = dStrat.evaluate(dealerHand->calculateBestScore());
					if (dMove == BlackjackMoves::Hit) {
						dealerHand->addToHand(deck->deal());
					}
				}
			}
			else {
				for (int i = 0; i < participants.size(); i++) {
					playerHands[i]->emptyHand(deck);
				}

				dealerHand->emptyHand(deck);
			}

				
		});

		

		//promptPlayer(participants.size() - 1);
		/*
		auto timeout = new GameObject();

		ControllerFactory::create(scene, [this, players, &timeout]() {
			if (!deck->empty()) {
				playerHands[deck->size() % players]->addToHand(deck->deal());
			}
		});

		//deck->reset();

		*/
	}
};

