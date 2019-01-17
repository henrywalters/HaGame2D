#pragma once
#include <iostream>
#include <string>

const int DEFAULT_BANK = 500;
const int BANK_LIMIT = 1000000;
const int BANK_FLOOR = -100;

class CasinoPlayer
{
	int bank;
	std::string name;
public:
	CasinoPlayer(std::string _name);
	~CasinoPlayer();

	bool canBet(int amount);

	void win(int amount);

	void lose(int amount);

	int getBank();

	std::string getName();
};

