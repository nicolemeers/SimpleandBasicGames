#include "../headers/Hangman.h"

/*
* This callback function gets called by libcurl as soon as there is data received that needs to 
* be saved. For most transfers, this callback gets called many times and each invoke delivers 
* another chunk of data. ptr points to the delivered data, and the size of that data is nmemb; 
* size is always 1.
* The data passed to this function is not null-terminated.
* To read more on this writecallback function, read the libcurl documentation:
* https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
*/
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData)
{
	// we can store everything in a large string and then parse it out later
	// this way, we don't have to define a memory block and allocate to it
	size_t totalSize = size * nmemb;
	userData->append((char*)contents, totalSize);
	return totalSize;
}

void Hangman::initWordBank()
{
	/*
	* There are two ways to transfer from a vector to a set
	* The first is copy-construct. The second is move-construct.
	* Move should be as good/fast as copy or better
	*/

	wordBank = std::set<std::string>(words.begin(), words.end());

	// moving words over removes them from the vector. 
	// this line of Moving the words over at the end removes previously added data:
	//wordBank = set<string>(make_move_iterator(words.begin()), make_move_iterator(words.end()));

}

/*
* A note on sending the header information over:
* 		// Use CURLOPT_HEADEROPT to make the headers only get sent to where you intend them to get sent.
		// CURLOPT_HTTPHEADER will send all information, and it will be accessible to where it's sent
		// ensure that either the header isn't sensitive, or use different parameter
		// Using CURLOPT_HEADEROPT gives an error of missin API key
*/
void Hangman::readContent()
{
	// this will be using libcurl to fill out our selection of words

	// current curl handle to store information
	CURL* curl;
	CURLcode res;
	// buffer to store the response that we will parse out
	std::string readWordsIn;
	// the API key we will need to pass in
	const std::string apiKey = "kQ42SFV0U4OBvkhl/Xs4bA==ydvsN2xplko210hC";
	// word to store the current word we are passing to the thesaurus
	std::string word = "";

	// we do not need to do global curl init
	
	for (auto iter = words.begin(); iter != words.end(); iter++)
	//for (int i = 0; i < words.size(); i++)
	{
		// reset our string for the next word
		readWordsIn = "";

		// initialize the current curl session
		curl = curl_easy_init();

		// get the current word for the API request
		word = *iter;
		//word = words[i];

		// construct the url with the word we are looking up
		const std::string apiUrl = "https://api.api-ninjas.com/v1/thesaurus?word=" + word;

		// tell libcurl how to behave: pass url pointer to work with, and set it up so we can pass the API Key
		curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());


		// we need to send the request headers so we can get the data from the API
		// in this case, we need to send the API key over
		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, ("X-Api-Key: " + apiKey).c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Set the callback function to write response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readWordsIn);

		// send the URL/HTTP request
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			// cerr means character error; use cerr to display errors
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n"; // endl;
		}
		else
		{
			// we were able to get the response back
			// we need to call our parser function (there is a class that can handle it, apparently?? we will write our own)
			parserFunction(readWordsIn);
		}

		// cleanup the current session
		curl_easy_cleanup(curl);
		//The list should be freed again(after usage) with curl_slist_free_all (the linked list of strings is added to with curl_slist_append)
		curl_slist_free_all(headers);
	}

	/* always cleanup */	
	curl_global_cleanup();
}

void Hangman::parserFunction(std::string rawBuffer)
{
	// we know we get the response in JSON format, and need to parse it out

	std::string wordToken = "temp";
	std::string formatToken = "";
	std::stringstream parseS(rawBuffer);

	// print out the sstream
	//cout << parseS.str() << endl;

	// deal with the opening format of the response
	std::getline(parseS, formatToken, '[');
	while (wordToken != "")
	{
		// skip the quotation marks
		std::getline(parseS, formatToken, '"');

		// check if there is a bracket
		if (formatToken.find("]") != std::string::npos)
		{	
			// check if it's the end or the antonym switch
			if (formatToken.find("}") != std::string::npos)
				break;

			// antonym switch - skip formatting and get to the next 
			getline(parseS, formatToken, '[');
			getline(parseS, formatToken, '"');
		}
		
		// get the word
		getline(parseS, wordToken, '"');

		// check the word for weird tokens -> do not add (some words have accented letters, which are displayed with numbers and slashes in the string)
		for (int i = 0; i < wordToken.size(); i++)
		{
			if (!isalpha(wordToken.at(i)))
				continue;
		}

		// add the word into our set
		wordBank.emplace(wordToken);

	}

}

void Hangman::wordBankHandler()
{	
	// warning
	std::cout << "Loading word bank...";
	
	// move the base words over into our word bank
	initWordBank();
	
	// pull words using the Thesaurus API
	readContent();

	std::cout << "\n\nDone!";

	// Pause for 2 seconds before continuing
	Sleep(2000);
}

void Hangman::gameExplanation() const
{
	// clear text from above
	system("cls");

	std::cout << "\nWelcome to Hangman!\n\n";

	std::cout << "Here are the rules to play: \n\n";
	std::cout << "1. A random word will be chosen, but you will not know the word.\n";
	std::cout << "2. You will have to guess the word by guessing the letters in the word.\n";
	std::cout << "3. You will have 12 chances to guess the word.\n";
	std::cout << "4. If you guess the word correctly, you win. If you run out of chances, you hang.\n";
	std::cout << "5. Good luck! Let's begin! \n\n";

	system("pause");
}

void Hangman::startState()
{
	// make sure everything is reset/cleared
	newGame = false;
	isVictory = false;
	chances = 12;
	currentHangmanWord.clear();
	guessedHangmanWord.clear();
	guessed.clear();


	// we need to select our word randomly
	srand(time(0));
	int randIndex = rand() % wordBank.size();

	// set uses iterator
	// we will use std::next to advance the iterator to the right index/position
	auto iter = std::next(wordBank.begin(), randIndex);

	// update the word we are using
	currentHangmanWord = *iter;
	// set up the guessed word
	guessedHangmanWord = *iter;

	// we need to grey up our guess state
	for (char& c : guessedHangmanWord) 
	{
		// we want to leave in hyphons and spaces
		if (c == '-' || c == ' ')
			continue;
		c = '_';
	}

}

bool Hangman::guessingState()
{
	// clear existing text
	system("cls");

	// show current hangman state
	drawHangman();

	// ask for a guess
	char guess;
	std::cout << "\nPlease enter a guess: ";
	std::cin >> guess;

	// make sure the guess is good
	if (isValid(guess))
	{
		updateHangman(guess);
	}
	else
	{
		std::cout << "\n\nThat letter is incorrect.\n";
		chances--;
		// wait for the user to click to continue
		system("pause");
	}

	// update our guessed words
	guessed[guess] = true;

	// check if the game is over (win/lose)
	if (chances == 0 || guessedHangmanWord.find("_") == std::string::npos)
	{
		if (chances > 0)
		{
			isVictory = true;
		}
		return false;
	}

	return true;
}


void Hangman::endState()
{
	// show final hangman state
	system("cls");
	drawHangman();

	if (isVictory)
	{
		std::cout << "\nCONGRATULATIONS\n\n";
	}
	else
	{
		std::cout << "\nOut of chances - You've hung!\n\nThe word was : " << currentHangmanWord;
		std::cout << "\n\nGAME OVER\n\n";
	}

	// ask if they'd like to play a new game
	std::string response = "";
	std::cout << "\nWould you like to play another hangman?(y/n) ";
	std::cin >> response;

	// to convert to lowercase:
	// rangebased loop, tolower each char
	// transform
	// for_each
	transform(response.begin(), response.end(), response.begin(), ::tolower);

	while (response != "y" && response != "n")
	{
		std::cout << "\nWould you like to play another game? (y/n)";
		std::cin >> response;
		transform(response.begin(), response.end(), response.begin(), ::tolower);
	}

	if (response == "y")
		newGame = true;
}

bool Hangman::hasUsed(char guess)
{
	if (guessed[guess])
		return true;

	return false;
}

bool Hangman::isValid(char& guess)
{
	// first, check if the guess is a valid entry
	while (!isalpha(guess))
	{
		std::cout << "\nThis is not a valid guess, please enter a valid guess: ";
		std::cin >> guess;
	}

	// check if the word has been entered already
	while (hasUsed(guess))
	{
		std::cout << "\nYou have already guessed this letter, please enter a different guess: ";
		std::cin >> guess;
	}

	// now we need to check against the word we're guessing for
	if (currentHangmanWord.find(std::string(1,guess)) != std::string::npos)
		return true;

	return false;
}

void Hangman::updateHangman(char guess)
{
	// get all indices the letter appears in
	std::vector<int> indices;
	for (int i = 0; i < currentHangmanWord.length(); i++)
	{
		if (guess == currentHangmanWord.at(i))
		{
			indices.push_back(i);
		}
	}

	// update the guess state
	for (int i : indices)
	{
		guessedHangmanWord.at(i) = guess;
	}

}

void Hangman::drawHangman() const
{
	std::cout << "Guesses remaining: " << chances << "\n\n";
	
	std::cout << "Current word:\n";

	for (char c : guessedHangmanWord)
	{
		std::cout << c << " ";
	}
	std::cout << "\n";

	if (guessed.size() > 0)
	{
		std::cout << "Guessed letters:\n";
		
		for (auto iter = guessed.begin(); iter != guessed.end(); iter++)
		{
			std::cout << iter->first;
			auto temp = std::next(iter);
			if (temp != guessed.end())
			{
				std::cout << ", ";
			}
			else
			{
				std::cout << "\n";
			}
		}
	}
}

Hangman::Hangman()
{
	// do nothing
	isVictory = false;
	wordBank = {};
	chances = 12;
	newGame = false;
	guessed = {};
}

void Hangman::hangManInit()
{
	// we need to have a function that gets the word bank to choose from
	//	have a handler for the wordBank
	wordBankHandler();

	// we need to explain the game
	gameExplanation();

}

bool Hangman::gameLoop()
{
	// loop the game in case of new game
	do
	{
		// reset the new game condition
		newGame = false;

		// set up the game 
		startState();

		// game loop
		while (guessingState()) {}

		// win/lose state
		endState();


	} while (newGame);

	std::cout << "\nExiting game...\n";

	return false;
}

void Hangman::shutdown()
{
	// reset
	isVictory = false;
	//wordBank = {};
	chances = 12;
	newGame = false;
	guessed = {};
}

Hangman::~Hangman()
{
	// do nothing
}


