#include "Board.h"
#include <string>
#pragma once
class Player
{
private:
	char symbol;
	int numTurns;
	bool isTurn;
	bool isRandom;
	bool exit;
	std::string playerName;

	// switch to current turn/opposite turn
	void switchTurn();

public:

	// init player (start, symbol, etc...)
	Player();

	// call this to init the player
	void playerInit(char setSymol, bool startTurn, bool random, std::string name);

	// we could do player move in here
	void playTurn(Board& game);

	std::string getName() const;
	bool getExitCondition() const;
	int getNumTurns() const;

	void shutdown();

	~Player();

};

