#pragma once
#include "HaGame2D.h"
#include "ButtonMenu.h"
#include "CasinoGame.h"

const enum Menu {
	Main,
	ChooseGame,
	ReplayGame,
	PlayerCount,
	Bet
};

const enum MainMenuOption {
	NewCareer,
	LoadCareer,
	Settings,
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
	MenuItem<MainMenuOption> {"Load Previous Adventure", MainMenuOption::LoadCareer},
	MenuItem<MainMenuOption> {"Settings", MainMenuOption::Settings},
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
	MenuItem<int> {"Walk Away", 0}
};

template <class T>
class CasinoMenu : public CasinoGame<T>, Promise<T> {
	
public:

	std::vector<MenuItem<T>> items;

	CasinoMenu(std::vector<MenuItem<T>> _items, std::string _name) : CasinoGame<T>(_name), Promise<T>() {
		name = _name;
		items = _items;
	}

	Promise<T> play(Resolution<T> resolution, std::vector<Participant> participants) {
		GameObject *object = scene->instantiate(new GameObject());
		auto menuContainer = object->add();
		auto menu = menuContainer->addComponent(new ButtonMenu<T>(items));
		menuContainer->move(Vector(500 - BTN_WIDTH / 2, 50));
		
		object->addComponent(new BoxRenderer(1000, 1000, true, Color::green()));
		menu->selectOptionFunc = [this, resolution](T value) {
			std::cout << "Selected option: " << value << "\n";
			resolve(resolution, value);
		};

		return *this;
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
};