#include "../headers/Player.h"

Player::Player()
{
	symbol = 0;
	numTurns = -1;
	isTurn = false;
	isRandom = false;
	exit = false;
	playerName = "";
}

void Player::playerInit(char setSymbol, bool startTurn, bool random, std::string name)
{
	symbol = setSymbol;
	numTurns = 0;
	isTurn = startTurn;
	isRandom = random;
	playerName = name;
	exit = false;
}

void Player::switchTurn()
{
	isTurn = !isTurn;
	numTurns++;
}

void Player::playTurn(Board& game)
{
	std::string temp = "";
	int position = -1;
	if (!isRandom)
	{
		// get the position that we need
		std::cout << "Please enter your move: ";
		std::cin >> temp;
		std::cout << "\n";

		// check for the exit condition
		if (temp == "exit")
		{
			exit = true;
			return;
		}
		position = stoi(temp);
	}
	else
	{
		// use a random number generator to get the board position: 
		// - set the random seed
		srand(time(0));

		// define the range
		int min = 0;
		int max = (game.getSize() * game.getSize()) - 1;

		// generate the random number
		position = rand() % (max - min + 1) + min;
		
	}
	// pass to the board to handle
	game.updateBoard(position, this->symbol, isRandom);

	// change turns immediately
	this->switchTurn();
}

std::string Player::getName() const
{
	return playerName;
}

bool Player::getExitCondition() const
{
	return exit;
}

int Player::getNumTurns() const
{
	return numTurns;
}

void Player::shutdown()
{
	// reset the players
	symbol = 0;
	numTurns = -1;
	isTurn = false;
	isRandom = false;
	exit = false;
	playerName = "";
}

Player::~Player()
{
	// do nothing
}
