#include "stdafx.h"
#include "CasinoAdventure.h"


CasinoAdventure::CasinoAdventure(int noOfPlayers)
{
	casino = new Game(WIDTH, HEIGHT, "Casino Adventure: a Gambling Experience");
	menus = new MenuManager(casino);

	mainMenu.setScene(casino);
	playerCountMenu.setScene(casino);
	replayGameMenu.setScene(casino);
	chooseGameMenu.setScene(casino);
	betMenu.setScene(casino);

	//addGame(blackjack);

	blackjack.setScene(casino);
	games["BlackJack"] = blackjack;


	if (noOfPlayers > 0 && noOfPlayers <= MAX_PLAYERS) {
		for (int i = 1; i <= noOfPlayers; i++) {
			CasinoPlayer player = CasinoPlayer("Player " + std::to_string(i));
			players.push_back(player);
		}
	} else {
		std::cout << "WARNING - invalid player count, must be: 0 < players <= " << MAX_PLAYERS << "\n";
	}
}


CasinoAdventure::~CasinoAdventure()
{
}
