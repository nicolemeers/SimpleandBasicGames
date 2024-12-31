#include "../headers/Board.h"

Board::Board(int size)
{
	// do nothing
}

void Board::boardInit(int size)
{
	this->size = size;
	boardGame = std::vector<char>(size * size, 0);
}

void Board::updateBoard(int position, char symbol, bool isRandom)
{

	if (!isRandom)
	{
		// first, check that the position is valid
		while (position < 0 || position >= (size * size))
		{
			std::cout << "Invalid position. Enter valid position: ";
			std::cin >> position;
			std::cout << "\n";
		}
		//std::cout << "\n";


		// second, check that the position is available
		while (boardGame[position] != 0)
		{
			std::cout << "This position is already taken. Enter new position: ";
			std::cin >> position;
			std::cout << "\n";
		}
	}
	else
	{
		// the position will always be in the correct range

		// check that the position is available
		while (boardGame[position] != 0)
		{
			//					rand() % (max - min + 1) + min;
			position = rand() % ((size * size - 1) - 0 + 1) + 0;
		}
	}

	// third, update the board
	boardGame[position] = symbol;
}

bool Board::isWinCondition() const
{
	bool checkWinStatus = false;
	// use to measure current win status
	char symbol;
	int oldRow = 0;
	// not the most efficient method to check
	// check for size in a row in each direction
	for (int i = 0; i < size * size; i++)
	{
		// for each position, check for the "size in a row"
		symbol = boardGame[i];
		if (symbol == 0)
			continue;
		
		// get the current row and row index that we are currently in
		int curRow = i / size;
		int curRowIndex = curRow * size;
		int endCurRowIndex = (curRow * size) + size;

		// check horizonatally
		// set j to the current row starting index
		for (int j = curRowIndex; j < endCurRowIndex; j++)
		{
			
			if (boardGame[j] != symbol)
				break;
			// if j is at the end of the row, then we have a full row of one symbol
			if (j == endCurRowIndex - 1)
				return true;

			// go to the next row
			if (curRow > 0)
				i += size;
		}
		
		// we only need to do this for one row
		// check veritcally
		if (curRow == 0)
		{
			int curCol = i - curRowIndex;
			// set j to the current column
			for (int j = curCol; j < size * size; j += size)
			{
				if (boardGame[j] != symbol)
					break;
				// if j is at the end last column, there is a full column of one symbol
				if (j + size >= size * size)
					return true;
			}
		}

	}
	// check these outside of the loop because we know there are only two cases where there will
	// be a diagonal win, it is a waster of time to check cases that will never be valid
	
	// check diagnocally up
	// start from the bottom rows
	int startIndex1 = size * (size - 1);
	symbol = boardGame[startIndex1];
	for (int i = 0; i < size; i++)
	{
		if (symbol == 0)
			break;
		if (boardGame[startIndex1] != symbol)
			break;
		startIndex1 = (startIndex1 - size) + 1;
		if (i == 2)
			return true;
	}
	symbol = boardGame[boardGame.size() - 1];
	for (int i = (size * size) - 1; i >= 0; i = i - size - 1)
	{
		if (symbol == 0)
			break;
		if (boardGame[i] != symbol)
			break;
		if (i == 0)
			return true;
	}

	// check diagnocally down
	// start from the top row
	symbol = boardGame[0];
	for (int i = 0; i < boardGame.size(); i+= i + size + 1)
	{
		if (symbol == 0)
			break;;
		if (boardGame[i] != symbol)
			break;
		if (i == boardGame.size() - 1)
			return true;

	}
	symbol = boardGame[size - 1];
	for (int i = size - 1; i < boardGame.size(); i = i + (size - 1))
	{
		if (symbol == 0)
			break;;
		if (boardGame[i] != symbol)
			break;
		if (i == (size * (size - 1)))
			return true;

	}

	return false;
}

bool Board::isTie(int numTurns) const
{
	// numturns is the total number of turns taken
	if (!isWinCondition())
	{
		if (numTurns >= (size * size))
		{
			return true;
		}
	}
	return false;
}

void Board::showBoardPositions() const
{
	// show board numbers
	/*std::cout << "Board place numbers: \n";
	std::cout << " 1 | 2 | 3\n";
	std::cout << "-----------\n";
	std::cout << " 4 | 5 | 6\n";
	std::cout << "-----------\n";
	std::cout << " 7 | 8 | 9\n\n";*/

	// change this for zero position and to reflect the size of the vector
	std::cout << "Board place numbers: \n";
	for (int i = 0; i < boardGame.size(); i++)
	{
		if ((i+1) % size == 0 && i != ((size * size) - 1) && i > 0)
		{
			std::cout << " " << i << "\n";
			std::cout << "-----------\n";
			continue;
		}
		else if (i == (size * size) - 1)
		{
			std::cout << " " << i << "\n\n";
			continue;
		}
		std::cout << " " << i << " |";
	}
}

void Board::showBoardState() const
{
	// show current board
	for (int i = 0; i < (size * size); i++)
	{
		std::cout << " ";
		if (boardGame[i] == 0)
		{
			std::cout << "  ";
		}
		else
		{
			std::cout << boardGame[i] << " ";
		}
		if (((i + 1) % size) == 0)
		{
			std::cout << "\n";
			if (i < (size * size) - 1)
				std::cout << "-----------\n";
		}
		else
		{
			std::cout << "|";
		}
	}
}

int Board::getSize() const
{
	return size;
}

void Board::shutdown()
{
	boardGame.clear();
}

Board::~Board()
{
	// do nothing
}
