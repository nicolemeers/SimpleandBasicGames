#include "Hangman.h"

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

	m_wordBank = std::set<std::string>(m_words.begin(), m_words.end());

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
	auto setEnd = m_words.end();
	for (auto iter = m_words.begin(); iter != setEnd; iter++)
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
	getline(parseS, formatToken, '[');
	while (wordToken != "")
	{
		// skip the quotation marks
		getline(parseS, formatToken, '"');

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
		m_wordBank.emplace(wordToken);

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
	m_newGame = false;
	m_isVictory = false;
	m_chances = 12;
	m_currentHangmanWord.clear();
	m_guessedHangmanWord.clear();
	m_guessed.clear();


	// we need to select our word randomly
	srand(time(0));
	int randIndex = rand() % m_wordBank.size();

	// set uses iterator
	// we will use std::next to advance the iterator to the right index/position
	auto iter = next(m_wordBank.begin(), randIndex);

	// update the word we are using
	m_currentHangmanWord = *iter;
	// set up the guessed word
	m_guessedHangmanWord = *iter;

	// we need to grey up our guess state
	for (char& c : m_guessedHangmanWord) 
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
		m_chances--;
		// wait for the user to click to continue
		system("pause");
	}

	// update our guessed words
	m_guessed[guess] = true;

	// check if the game is over (win/lose)
	if (m_chances == 0 || m_guessedHangmanWord.find("_") == std::string::npos)
	{
		if (m_chances > 0)
		{
			m_isVictory = true;
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

	if (m_isVictory)
	{
		std::cout << "\nCONGRATULATIONS\n\n";
	}
	else
	{
		std::cout << "\nOut of chances - You've hung!\n\nThe word was : " << m_currentHangmanWord;
		std::cout << "\n\nGAME OVER\n\n";
	}

	// ask if they'd like to play a new game
	char response = 0;
	std::cout << "\nWould you like to play another hangman? (y/n) ";
	std::cin >> response;

	///// DAD //////////////
	/*stricmp(response, "y");*/
	while (response != 'y' && response != 'Y' && response != 'n' && response != 'N')
	{
		std::cout << "\nWould you like to play another hangman? (y/n) ";
		std::cin >> response;
	}
	////////////////////////////
	// 

	if (response == 'y')
		m_newGame = true;
}

const bool Hangman::hasUsed(char guess) const
{
	//if (m_guessed[guess])
		//return true;
	if (m_guessed.find(guess) != m_guessed.end())
		return true;

	return false;
}

const bool Hangman::isValid(char& guess) const // change is valid so it can be const
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
	if (m_currentHangmanWord.find(std::string(1,guess)) != std::string::npos)
		return true;

	return false;
}

void Hangman::updateHangman(char guess)
{
	// get all indices the letter appears in
	std::vector<int> indices;
	for (int i = 0; i < m_currentHangmanWord.length(); i++)
	{
		if (guess == m_currentHangmanWord.at(i))
		{
			indices.push_back(i);
		}
	}

	// update the guess state
	for (int i : indices)
	{
		m_guessedHangmanWord.at(i) = guess;
	}

}

void Hangman::drawHangman() const
{
	std::cout << "Guesses remaining: " << m_chances << "\n\n";
	
	std::cout << "Current word:\n";

	for (char c : m_guessedHangmanWord)
	{
		std::cout << c << " ";
	}
	std::cout << "\n";

	if (m_guessed.size() > 0)
	{
		std::cout << "Guessed letters:\n";
		// create local cashed copy (otherwise, we will a copy every time in our for loop; we only want to do this for removing elements)
		auto end = m_guessed.end();
		for (auto iter = m_guessed.begin(); iter != end; iter++)
		{
			std::cout << iter->first;
			auto temp = next(iter);
			if (temp != end)
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

Hangman::Hangman() : m_isVictory(false), m_chances(12), m_newGame(false)
{
	// don't do this, do an initilizer list
	/*
	isVictory = false;
	wordBank = {};
	m_chances = 12;
	m_newGame = false;
	guessed = {};
	*/
}

void Hangman::hangManInit()
{
	// set up our vector (our base words)
	m_words = 
	{	"syzygy", "abatis", "controvert", "controversy", "contuse", "luminary", "overt", 
		"narcissism", "happy", "elegant", "poor", "bee", "helicopter", "fast", "fortuitous", 
		"sequacious", "lascivious", "lecherous", "pulchritudinous", "ineffable", "jubilee", 
		"jazz", "rythm", "capricious", "avarice", "greed", "serendipitous", "misanthrope",
		"taciturn", "aloof", "callous", "cynical", "callow", "sad", "gallows", "perhaps", 
		"melifluous","sonorous", "egress", "somber", "solemn", "gallant", "dour", "din", 
		"eccentric", "epitome", "inundate", "deluged", "frenetic", "dessicated", "wry", "sly", 
		"wend", "gallant", "salacious", "scrupulous", "dubious", "objurgate", "happy", "sad",
		"splendid", "sublime", "monotonous", "melancholic", "melodious", "bird", "songbird",
		"rhythm", "silly", "boring"
	};


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
		m_newGame = false;

		// set up the game 
		startState();

		// game loop
		while (guessingState()) {}

		// win/lose state
		endState();


	} while (m_newGame);

	std::cout << "\nExiting game...\n";

	return false;
}

void Hangman::shutdown()
{
	// reset
	m_isVictory = false;
	//m_wordBank = {};
	m_chances = 12;
	m_newGame = false;
	m_guessed = {};
}

Hangman::~Hangman()
{
	// do nothing
}


