#pragma once
#include "HaGame2D.h"
#include "CasinoMenus.h"
#include "BlackJack.h"
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

	std::vector<Participant> participants;

	std::string currentGame;

	std::unordered_map<std::string, CasinoGame<void>> games;

	CasinoMenu<MainMenuOption> mainMenu = CasinoMenuFactory::MainMenu();
	CasinoMenu<int> playerCountMenu = CasinoMenuFactory::PlayerCount();
	CasinoMenu<int> replayGameMenu = CasinoMenuFactory::ReplayGame();
	CasinoMenu<CasinoGameTypes> chooseGameMenu = CasinoMenuFactory::ChooseGame();

	BettingMenu betMenu = CasinoMenuFactory::Bet();

	BlackJack blackjack;

	bool running = true;

	void playerCount() {
		auto resolve = Resolution<int>();

		resolve.then([&](int playerCount) {
			menus->closeMenu(Menu::PlayerCount);
			players.clear();
			
			for (int i = 0; i < playerCount; i++) {
				players.push_back(CasinoPlayer("Player " + std::to_string(i + 1)));
			}

			chooseGame();
		});

		playerCountMenu.play(resolve);
		menus->openMenu(Menu::PlayerCount);
	}

	void settings() {

	}

	void loadGame() {

	}

	void chooseGame() {
		auto resolve = Resolution<CasinoGameTypes>();

		resolve.then([this](CasinoGameTypes game) {
			menus->closeMenu(Menu::ChooseGame);
			currentGame = CASINO_GAME_STRINGS[game];
			captureBets();
		});

		chooseGameMenu.play(resolve);

		menus->openMenu(Menu::ChooseGame);
	}

	void replayGame() {
		auto resolve = Resolution<int>();
		
		resolve.then([this](int replay) {
				menus->closeMenu(Menu::ReplayGame);
				if (replay == 0) {
					chooseGame();
				}
				else if (replay == 1) {
					replayGame();
				}
				else {
					mainGameMenu();
				}
		});

		replayGameMenu.play(resolve);
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
				/*case MainMenuOption::Settings:
					settings();
					break;
				case MainMenuOption::LoadCareer:
					loadGame();
					break;
					*/
				}
				
			})
			.except([this]() {
				menus->closeMenu(Menu::Main);
				std::cout << "Failed to handle main menu for some reason";
			});

		mainMenu.play(resolution);

		menus->openMenu(Menu::Main);
	}

	void playGame(std::string game) {
		if (games.find(game) != games.end()) {

			casino->activateScene(game);

			casino->prepareScene();
			
			auto resolve = Resolution<void>();
			resolve.then([this, game] () {
				std::cout << "Finished playin blackjack";
				casino->deactivateScene(game);
				mainGameMenu();
			});

			blackjack.setParticipants(participants);
			blackjack.play(resolve);

			
		}
		else {
			std::cout << "WARNING - Game: " << game << " does not exist\n";
		}
	}

	template<class T>
	void addGame(CasinoGame<T> game) {
		if (games.find(game.getName()) == games.end()) {
			games[game.getName()] = game;
			games[game.getName()].setScene(casino);
			std::cout << "Added game: " << game.getName() << "\n";
		}
	}

	void captureBets() {

		participants.clear();

		for (CasinoPlayer player : players) {
			Participant participant = Participant{ player, 0 };
			std::cout << "Player: " << player.getName() << "\n";
			participants.push_back(participant);
		}

		auto resolve = Resolution<void>();
		resolve.then([this]() {
			menus->closeMenu(Menu::Bet);
			playGame(currentGame);
		});

		betMenu.setParticipants(participants);
		betMenu.play(resolve);

		menus->openMenu(Menu::Bet);
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

