#include "TicTacToeGame.h"
#include "Hangman.h"

#pragma once
class MenuManager
{
private:
	// games we run
	TicTacToeGame ticTacToe;

	Hangman hangman;

	bool runGame;
	//bool quit;

	int menuOptions() const;
	void playHangman();
	void playTicTacToe();
	bool playNewGame();

public:
	MenuManager();

	void menuLoop();

	~MenuManager();
};

