#include "stdafx.h"
#include "CasinoPlayer.h"


CasinoPlayer::CasinoPlayer(std::string _name)
{
	name = _name;
}


CasinoPlayer::~CasinoPlayer()
{
}

bool CasinoPlayer::canBet(int amount)
{
	return amount >= 0 && bank - amount >= BANK_FLOOR;
}

void CasinoPlayer::win(int amount)
{
	if (amount >= 0) {
		bank += amount;

		if (bank > BANK_LIMIT) {
			bank = BANK_LIMIT;
		}
	}
	else {
		std::cout << "WARNING - Betting a negative number is a code error";
	}
}

void CasinoPlayer::lose(int amount) {
	if (amount >= 0) {
		bank -= amount;

		if (bank < BANK_FLOOR) {
			bank = BANK_FLOOR;
		}
	}
	else {
		std::cout << "WARNING - Betting a negative number is a code error";
	}
}

int CasinoPlayer::getBank() {
	return bank;
}

std::string CasinoPlayer::getName() {
	return name;
}