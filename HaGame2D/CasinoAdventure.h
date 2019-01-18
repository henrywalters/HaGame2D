#pragma once
#include "HaGame2D.h"
#include "CasinoMenus.h"
//General Constants

const int WIDTH = 1000;
const int HEIGHT = 800;

const int MAX_PLAYERS = 4;



//Asset Locations

class MenuManager {
	Game *game;
	int openMenuIndex;
	bool menuIsOpen;
	std::unordered_map<std::string, bool> initialzedMenus;
public:
	MenuManager(Game *casino) {
		game = casino;
		openMenuIndex = 0;
		menuIsOpen = false;
	}

	void openMenu(Menu menu) {
		closeMenu((Menu)openMenuIndex);
		openMenuIndex = menu;
		game->activateScene(MENU_STRINGS[menu]);
		menuIsOpen = true;

		if (initialzedMenus.find(MENU_STRINGS[menu]) == initialzedMenus.end()) {
			game->prepareScene();
			initialzedMenus[MENU_STRINGS[menu]] = true;
		}

	}

	void closeMenu(Menu menu) {
		game->deactivateScene(MENU_STRINGS[menu]);
		//game->resetScene(MENU_STRINGS[menu]);
		menuIsOpen = false;
	}

	Scene *get(Menu menu) {
		return game->getScene(MENU_STRINGS[menu]);
	}
};

class CasinoAdventure
{
private:
	Game *casino;
	MenuManager *menus;

	std::vector<CasinoPlayer> players;

	std::unordered_map<std::string, CasinoGame<void>> games;

	CasinoMenu<MainMenuOption> mainMenu = CasinoMenuFactory::MainMenu();
	CasinoMenu<int> playerCountMenu = CasinoMenuFactory::PlayerCount();
	CasinoMenu<int> replayGameMenu = CasinoMenuFactory::ReplayGame();

	bool running = true;

	void playerCount() {
		std::vector<Participant> participants;
		auto resolution = Resolution<int>()
			.then([this](int players) {
				menus->closeMenu(Menu::PlayerCount);
				replayGame();
			})
			.except([]() {

			});
		auto menu = playerCountMenu.play(resolution, participants);
		menus->openMenu(Menu::PlayerCount);
	}

	void settings() {

	}

	void loadGame() {

	}

	void replayGame() {
		std::vector<Participant> participants;
		auto resolution = Resolution<int>()
			.then([this](int replay) {
				menus->closeMenu(Menu::ReplayGame);
				if (replay == 0) {
					mainGameMenu();
				}
				else {
					mainGameMenu();
				}
			})
			.except([]() {

		});
		auto menu = replayGameMenu.play(resolution, participants);
		menus->openMenu(Menu::ReplayGame);
	}

	void mainGameMenu() {
		std::vector<Participant> participants;

		auto resolution = Resolution<MainMenuOption>()
			.then([this](MainMenuOption option) {
				menus->closeMenu(Menu::Main);
				switch (option) {
				case MainMenuOption::NewCareer:
					playerCount();
					break;
				case MainMenuOption::Quit:
					running = false;
					break;
				case MainMenuOption::Settings:
					settings();
					break;
				case MainMenuOption::LoadCareer:
					loadGame();
					break;
				}
				
			})
			.except([this]() {
				menus->closeMenu(Menu::Main);
				std::cout << "Failed to handle main menu for some reason";
			});

		auto menu = mainMenu.play(resolution, participants);

		menus->openMenu(Menu::Main);
	}

	void playGame(std::string game) {
		if (games.find(game) != games.end()) {
			casino->activateScene(game);

			std::vector<Participant> participants = captureBets();

			//games[game].play(participants);

			casino->resetScene(game);
			casino->deactivateScene(game);
		}
		else {
			std::cout << "WARNING - Game: " << game << " does not exist\n";
		}
	}

	template<class T>
	void addGame(CasinoGame<T> game) {
		if (games.find(game.getName()) == games.end()) {
			game.setScene(casino);
			games[game.getName()] = game;
		}
	}

	std::vector<Participant> captureBets() {
		std::vector<Participant> participants;

		for (CasinoPlayer player : players) {
			Participant participant;
			participant.bet = 0;
			participant.player = &player;
			participants.push_back(participant);
		}

		if (games.find(MENU_STRINGS[Menu::Bet]) != games.end()) {
			menus->openMenu(Menu::Bet);

			//games[MENU_STRINGS[Menu::Bet]].play(participants);

			menus->closeMenu(Menu::Bet);
		}
		else {
			std::cout << "WARNING - Bet menu has not been created. Try addGame(CasinoGame betMenu).\n";
		}

		return participants;
	}


public:

	CasinoAdventure(int noOfPlayers);
	~CasinoAdventure();

	void loop() {

		casino->prepareScene();

		mainGameMenu();

		while (running && casino->running) {
			casino->tick();
		}
	}
};

