#include <vector>
#include <iostream>

#pragma once
class Board
{
private:
	// board size (ex, size = 3, means the board is a 3x3 board)
	int size = 3;

	std::vector<char> boardGame;

public:

	// what is the board going to do?
	// the board needs to hold all of the stored states
	// the board needs to make updates if they're applicable (available and in bounds)
	// the board can check its state

	Board(int size = 3);

	// call this to initialize
	void boardInit(int size = 3);

	// make additions
	void updateBoard(int position, char symbol, bool isRandom);

	// board logic
	// check win conditions
	bool isWinCondition() const;
	// remember, const is for when the function does not modify this objects state
	bool isTie(int numTurns) const;

	// we could have the board handle drawing the current state too
	void showBoardPositions() const;

	void showBoardState() const;

	int getSize() const;

	void shutdown();

	~Board();
};

