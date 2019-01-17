#pragma once
#include "HaGame2D.h"
#include "CasinoPlayer.h"

struct Participant {
	CasinoPlayer *player;
	int bet;
};

template<class T>
class CasinoGame
{
public:
	std::string name;
	Scene *scene;

	CasinoGame() {
		name = "empty game";
	}
	CasinoGame(std::string _name);
	~CasinoGame();

	virtual Promise<T> play(Resolution<T> resolution, std::vector<Participant> participants) {
		return Promise<T>();
	};

	void setScene(Game *casino);

	std::string getName() {
		return name;
	}
};

template <class T>
CasinoGame<T>::CasinoGame(std::string _name) {
	name = _name;
}

template <class T>
CasinoGame<T>::~CasinoGame() {

}

template <class T>
void CasinoGame<T>::setScene(Game *casino) {
	scene = casino->addScene(name, false);
	scene->setDisplayPort(0, 0, casino->screenWidth, casino->screenHeight);
}