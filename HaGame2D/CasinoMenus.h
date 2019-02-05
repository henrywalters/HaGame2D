#pragma once
#include "HaGame2D.h"
#include "ButtonMenu.h"
#include "CasinoGame.h"

const enum CasinoGameTypes {
	BLACKJACK
};

const std::vector<std::string> CASINO_GAME_STRINGS = {
	"BlackJack"
};

const enum Menu {
	Main,
	ChooseGame,
	ReplayGame,
	PlayerCount,
	Bet
};

const enum MainMenuOption {
	NewCareer,
	//LoadCareer,
	//Settings,
	Quit
};

const std::vector<std::string> MENU_STRINGS = {
	"Main Menu",
	"Choose Game",
	"Replay Game",
	"Player Count Menu",
	"Bet Menu"
};

const std::vector<MenuItem<MainMenuOption>> MAIN_MENU_LIST = {
	MenuItem<MainMenuOption> {"New Casino Adventure", MainMenuOption::NewCareer},
	//MenuItem<MainMenuOption> {"Load Previous Adventure", MainMenuOption::LoadCareer},
	//MenuItem<MainMenuOption> {"Settings", MainMenuOption::Settings},
	MenuItem<MainMenuOption> {"Quit", MainMenuOption::Quit}
};

const std::vector<MenuItem<int>> PLAYER_COUNT_LIST = {
	MenuItem<int> {"One Player", 1 },
	MenuItem<int> {"Two Players", 2},
	MenuItem<int> {"Three Players", 3},
	MenuItem<int> {"Four Players", 4}
};

const std::vector<MenuItem<int>> REPLAY_GAME_LIST = {
	MenuItem<int> {"Play Again?", 1},
	MenuItem<int> {"Walk Away", 0},
	MenuItem<int> {"Go Home", -1}
};

const std::vector<MenuItem<CasinoGameTypes>> CASINO_GAMES_LIST = {
	MenuItem<CasinoGameTypes> {CASINO_GAME_STRINGS[CasinoGameTypes::BLACKJACK], CasinoGameTypes::BLACKJACK }
};

template <class T>
class CasinoMenu : public CasinoGame<T> {

	GameObject *menuObject;
	bool initialized = false;
	
public:

	std::vector<MenuItem<T>> items;


	CasinoMenu(std::vector<MenuItem<T>> _items, std::string _name) : CasinoGame<T>(_name) {
		name = _name;
		items = _items;
	}

	void play(Resolution<T> resolution) {
		if (!initialized) {

			initialized = true;

			menuObject = scene->instantiate(new GameObject());
			auto menuContainer = menuObject->add();
			menuContainer->move(Vector(500, 50));
			auto menu = menuContainer->addComponent(new ButtonMenu<T>(items));
			
			menuObject->addComponent(new BoxRenderer(1000, 1000, true, Color(47, 132, 30).rgb));
			menu->selectOptionFunc = [this, resolution](T value) {
				std::cout << "Selected option: " << value << "\n";
				resolve(resolution, value);
			};
		}
	}
};

class BetComponent : public Component {

public:
	std::vector<Participant> participants;
	int playerIndex = 0;
	ButtonComponent *moreBtn;
	ButtonComponent *lessBtn;
	ButtonComponent *nextBtn;
	TextRenderer *betRenderer;
	TextRenderer *playerRenderer;
	std::function<void(std::vector<Participant>)> onCompleteFunc;

	int default_bet = 5;

	BetComponent(std::vector<Participant> _participants, TextRenderer *_betRenderer, TextRenderer *_playerRenderer, ButtonComponent *_moreBtn, ButtonComponent *_lessBtn, ButtonComponent *_nextBtn, std::function<void(std::vector<Participant>)> _onCompleteFunc) {
		participants = _participants;
		onCompleteFunc = _onCompleteFunc;
		betRenderer = _betRenderer;
		moreBtn = _moreBtn;
		lessBtn = _lessBtn;
		nextBtn = _nextBtn;
		playerRenderer = _playerRenderer;
	};

	void onCreate() {
		auto labelFactory = TextLabelFactory(gameScene, "../Assets/Fonts/Casino3D.ttf", 30, Color::black());

		moreBtn->onClickFunc = [&]() {
			participants[playerIndex].bet += default_bet;
			auto pos = betRenderer->transform->position;
			gameScene->destroy(betRenderer->transform);
			betRenderer->transform = new GameObject();
			auto label = betRenderer->transform->addComponent(new TextRenderer(100, 50));
			label->setFont("../Assets/Fonts/Casino3D.ttf");
			label->setFontSize(30);
			label->setMessage(std::to_string(participants[playerIndex].bet).c_str());
			label->setFontColor(Color::black());
			label->transform->setPosition(Vector(660, 100));
			std::cout << std::to_string(participants[playerIndex].bet) << "\n";
			gameScene->instantiate(betRenderer->transform);

			std::cout << participants[playerIndex].player.getName() << " bet: $" << participants[playerIndex].bet << "\n";
		};

		lessBtn->onClickFunc = [&]() {
			if (participants[playerIndex].bet - default_bet >= 0) {
				participants[playerIndex].bet -= default_bet;
				gameScene->destroy(betRenderer->transform);
				betRenderer->transform = new GameObject();
				auto label = betRenderer->transform->addComponent(new TextRenderer(100, 50));
				label->setFont("../Assets/Fonts/Casino3D.ttf");
				label->setFontSize(30);
				label->setMessage(std::to_string(participants[playerIndex].bet).c_str());
				label->setFontColor(Color::black());
				label->transform->setPosition(Vector(660, 100));
				gameScene->instantiate(betRenderer->transform);
				std::cout << std::to_string(participants[playerIndex].bet) << "\n";
				std::cout << participants[playerIndex].player.getName() << " bet: $" << participants[playerIndex].bet << "\n";
			}
		};

		nextBtn->onClickFunc = [&]() {
			
			playerIndex++;
			std::cout << "Playing next: " << playerIndex << " out of " << participants.size() << "\n";
			if (playerIndex >= participants.size()) {
				playerIndex = 0;
			}
			gameScene->destroy(playerRenderer->transform);

			playerRenderer->transform = new GameObject();

			auto label = playerRenderer->transform->addComponent(new TextRenderer(225, 50));

			label->setFont("../Assets/Fonts/Casino3D.ttf");
			label->setFontSize(30);
			label->setMessage(participants[playerIndex].player.getName() + ", place your bet: ");
			label->setFontColor(Color::black());
			label->transform->setPosition(Vector(275, 100));
			gameScene->instantiate(playerRenderer->transform);

			gameScene->destroy(betRenderer->transform);
			betRenderer->transform = new GameObject();
			auto label2 = betRenderer->transform->addComponent(new TextRenderer(50, 50));
			label2->setFont("../Assets/Fonts/Casino3D.ttf");
			label2->setFontSize(30);
			label2->setMessage("0");
			label2->setFontColor(Color::black());
			label2->transform->move(Vector(660, 100));
			gameScene->instantiate(betRenderer->transform);

			if (playerIndex == 0) {
				onCompleteFunc(participants);
			}
		};
		
	}

	void update() {

	}
};

class BettingMenu : public CasinoGame<void>{
	bool initialized = false;

	TextRenderer *betLabel;
	TextRenderer *playerLabel;
	ButtonComponent *moreBtn;
	ButtonComponent *lessBtn;
	ButtonComponent *nextBtn;
public:

	BettingMenu(std::string _name) : CasinoGame<void>(_name) {};

	void play(Resolution<void> resolution) {
		std::cout << "Playing bet menu\n";
		if (hasParticipants) {
			if (!initialized) {
				initialized = true;

				auto betMenu = new GameObject();

				auto labelFactory = TextLabelFactory(scene, "../Assets/Fonts/Casino3D.ttf", 30, Color::black());

				auto bg = betMenu->add();
				bg->addComponent(new BoxRenderer(1000, 1000, true, Color(47, 132, 30).rgb));
				bg->z_index = 1;

				playerLabel = labelFactory.build("Player 1, Place your bet: ", Vector(350, 100), Vector(225, 50))->getComponent<TextRenderer>();
				betLabel = labelFactory.build("0", Vector(scene->screenWidth / 2 + 125, 100), Vector(100, 50))->getComponent<TextRenderer>();

				moreBtn = betMenu->add()->setPosition(Vector(scene->screenWidth / 2 + 50, 150))->addComponent(new ButtonComponent(50, 50));
				lessBtn = betMenu->add()->setPosition(Vector(scene->screenWidth / 2 - 100, 150))->addComponent(new ButtonComponent(50, 50));

				moreBtn->color = Color(47, 132, 30).rgb;
				lessBtn->color = Color(47, 132, 30).rgb;
			
				labelFactory.build("More", Vector(scene->screenWidth / 2 + 50, 175), Vector(50, 25));
				labelFactory.build("Less", Vector(scene->screenWidth / 2 - 100, 175), Vector(50, 25));

				nextBtn = betMenu->add()->move(Vector(scene->screenWidth / 2 + 200, 175))->addComponent(new ButtonComponent(50, 50));

				nextBtn->color = Color(47, 132, 30).rgb;

				labelFactory.build("Bet", Vector(scene->screenWidth / 2 + 200, 175), Vector(50, 50));
	
				scene->instantiate(betMenu);
			}
			int amt = 0;
			int player = 0;

			auto betController = new GameObject();
			betController->addComponent(new BetComponent(participants, betLabel, playerLabel, moreBtn, lessBtn, nextBtn, [=](std::vector<Participant> _participants) {
				participants = _participants;
				resolve(resolution);
			}));
			
			scene->instantiate(betController);
		}

		else {
			std::cout << "ERROR - betting menu requires participants be set. try setParticipants()\n";
		}
	}
};

class CasinoMenuFactory {
public:
	static CasinoMenu<MainMenuOption> MainMenu() {
		return CasinoMenu<MainMenuOption>(MAIN_MENU_LIST, MENU_STRINGS[Menu::Main]);
	}

	static CasinoMenu<int> PlayerCount() {
		return CasinoMenu<int>(PLAYER_COUNT_LIST, MENU_STRINGS[Menu::PlayerCount]);
	}

	static CasinoMenu<int> ReplayGame() {
		return CasinoMenu<int>(REPLAY_GAME_LIST, MENU_STRINGS[Menu::ReplayGame]);
	}

	static CasinoMenu<CasinoGameTypes> ChooseGame() {
		return CasinoMenu<CasinoGameTypes>(CASINO_GAMES_LIST, MENU_STRINGS[Menu::ChooseGame]);
	}

	static BettingMenu Bet() {
		return BettingMenu(MENU_STRINGS[Menu::Bet]);
	}
};