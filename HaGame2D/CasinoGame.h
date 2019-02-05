#pragma once
#include "HaGame2D.h"
#include "CasinoPlayer.h"

struct Participant {
	CasinoPlayer player;
	int bet;
};

template<class T>
class CasinoGame: public Promise<T>
{

protected:
	
	bool hasParticipants = false;
	std::vector<Participant> participants;
public:
	std::string name;
	Scene *scene;

	CasinoGame() : Promise<T>() {
		name = "empty game";
	}
	CasinoGame(std::string _name);
	~CasinoGame();

	virtual void play(Resolution<T> resolution) {};

	void setScene(Game *casino);

	void setParticipants(std::vector<Participant> _participants);

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
	std::cout << "Adding Scene: " << name << "\n";
	scene = casino->addScene(name, false);
	scene->setDisplayPort(0, 0, casino->screenWidth, casino->screenHeight);
}

template<class T>
inline void CasinoGame<T>::setParticipants(std::vector<Participant> _participants)
{
	participants = _participants;
	hasParticipants = true;
}
