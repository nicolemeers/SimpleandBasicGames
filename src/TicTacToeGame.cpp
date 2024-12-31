#include "../headers/TicTacToeGame.h"

void TicTacToeGame::turnState()
{
	// show current board state
	std::cout << "Current board state: \n";
	this->board.showBoardState();

	// player turn
	std::cout << "\nPlayer " << curPlayer << " is up!\n\n";

	switch (curPlayer)
	{
	case 1:
		player1.playTurn(board);
		break;
	case 2:
		player2.playTurn(board);
		break;
	default:
		std::cout << "error\n";
	}
	// check for the exit condition
	if (player1.getExitCondition() || player2.getExitCondition())
	{
		exit = true;
		return;
	}

	// show updated changes
	std::cout << "Player " << curPlayer << "'s changes: \n";
	this->board.showBoardState();

}

void TicTacToeGame::changeTurn()
{
	switch (curPlayer)
	{
	case 1:
		curPlayer = 2;
		break;
	case 2:
		curPlayer = 1;
		break;
	default:
		break;
	}

	std::cout << "\n\n";
	// click to continue
	system("pause");

	// clear text from above
	system("cls");
}

bool TicTacToeGame::checkEndState()
{
	// first, check exit condition
	if (exit)
		return false;

	// check tie condition
	if (board.isTie(player1.getNumTurns() + player2.getNumTurns()))
	{
		std::cout << "\n\nTIE GAME!!!\n\n";
		return false;
	}
	// check win condition
	if (board.isWinCondition())
	{
		std::cout << "\nPlayer " << curPlayer << " wins!!!\n\n";
		return false;
	}
	return true;
}


TicTacToeGame::TicTacToeGame()
{
	// do nothing
	//board.boardInit(3);
	//player1.playerInit('X', true, false, "Player 1");
	//player2.playerInit('O', false, true, "Player 2");
	//curPlayer = 1;
	//exit = false;
}

void TicTacToeGame::init()
{
	// set up the board;
	board.boardInit(3);
	player1.playerInit('X', true, false, "Player 1");
	player2.playerInit('O', false, true, "Player 2");
	curPlayer = 1;
	exit = false;

	// we need to prepare the game and explain how it works

	// 1. Greet the Game
	std::cout << "\nWelcome to Tic Tac Toe!\n\n";

	// 2. Explain the game rules
	std::cout << "The rules are simple: \n";
	std::cout << "1. Player 1 is X and Player 2 is O\n";
	std::cout << "2. To play, enter the place number \n";
	std::cout << "3. The first player to get 3 in a row wins!\n\n";

	// example --> draw the board with coordinate names
	//drawBoardNumbers();
	board.showBoardPositions();

	std::cout << "If you would like to exit the game, enter \"exit\" (without quotes)\n\n";

	// wait for user to click to continue;
	//cout << "Press any key to continue\n";
	system("pause");

	// clear text from above
	system("cls");

	// 3. start the game setup
	std::cout << "Starting the game...\n\n";
}

bool TicTacToeGame::run()
{
	// display the game
	this->board.showBoardPositions();

	// player turn
	this->turnState();

	// check end states
	if (!this->checkEndState())
		return false;

	// switch player
	this->changeTurn();
	
	return true;
}

void TicTacToeGame::shutdown()
{
	std::cout << "Shutting down game...\n\n";

	// reset
	board.shutdown();
	player1.shutdown();
	player2.shutdown();
	curPlayer = 1;
	exit = false;

}

TicTacToeGame::~TicTacToeGame()
{
	// do nothing
}
