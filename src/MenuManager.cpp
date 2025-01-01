#include "../headers/MenuManager.h"

int MenuManager::menuOptions() const
{
	int choice = -1;
	std::cout << "\nPick which game you'd like to play!\n\n";
	std::cout << "Options: ";
	std::cout << "\n\n1. Hangman";
	std::cout << "\n\n2. Tic Tac Toe";
	std::cout << "\n\n3. Exit (quit menu)";
	std::cout << "\n\nYour choice: ";
	std::cin >> choice;

	while (choice < 1 && choice > 2)
	{
		std::cout << "\nThat is not a valid option. Enter valid option: ";
		std::cin >> choice;
	}

	return choice;
}

void MenuManager::playHangman()
{
	hangman.hangManInit();

	hangman.gameLoop();
}

void MenuManager::playTicTacToe()
{
	ticTacToe.init();

	while (ticTacToe.run()) {}

	ticTacToe.shutdown();
}

bool MenuManager::playNewGame()
{
	char choice = 0;
	std::cout << "\nWould you like to play another game?(y/n) ";
	std::cin >> choice;

	while (choice != 'y' && choice != 'n')
	{
		std::cout << "\n\n\nWould you like to play another game?(y/n) ";
		std::cin >> choice;
	}

	if (choice == 'y')
		return true;

	return false;
}

MenuManager::MenuManager()
{
	// greeting
	std::cout << "Welcome!\n";

	runGame = false;
	//quit = false;
}

void MenuManager::menuLoop()
{
	int curGame = -1;
	do
	{
		// menu options, return game option
		curGame = menuOptions();

		// check exit
		if (curGame == 3)
		{
			// std::cout << "\nQuitting...";
			break;
		}

		std::cout << "\nBooting game...";
		Sleep(1500);
		system("cls");

		// case switch to execute game
		switch (curGame)
		{
		case 1:
			playHangman();
			break;

		case 2:
			playTicTacToe();
			break;
		/*case 3:
			quit = true;*/
		default:
			std::cout << "Error";
		}

		/*if (quit)
			break;*/

		system("pause");
			
		// check for quit
		runGame = playNewGame();

	} while (runGame);

	std::cout << "Quitting...";
}

MenuManager::~MenuManager()
{
	// do nothing
}
