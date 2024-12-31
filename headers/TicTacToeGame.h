#include "Player.h"
#include "Board.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>	
// #include <windows.h>
#include <ctime>

// using namespace std;

#pragma once
class TicTacToeGame
{
private:
	Board board;
	Player player1;
	Player player2;
	int curPlayer;
	bool exit;


	// start turn state for current player
	void turnState();
	// change turn state to the other player
	void changeTurn();
	// check end state for game
	bool checkEndState();

public:
	// constructor
	TicTacToeGame();

	// set up the game (ask player for icon symbol or something and explain game rules, get board size)
	void init();

	// run the game loop
	bool run();

	void shutdown();

	// destructor
	~TicTacToeGame();
};

