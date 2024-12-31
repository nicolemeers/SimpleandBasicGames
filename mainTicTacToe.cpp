#include "headers/TicTacToeGame.h"

int main()
{

	// run menu to pick which game we'd like to play


	// for now, only tictactoe
	TicTacToeGame ticTacToe;

	ticTacToe.init();

	while (ticTacToe.run()) {}

	ticTacToe.shutdown();

	return 0;
}