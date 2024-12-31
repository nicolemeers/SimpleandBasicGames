#include "headers/Hangman.h"


int main()
{
	Hangman hangmanGame;

	hangmanGame.hangManInit();

	hangmanGame.gameLoop();
	
	return 0;
}