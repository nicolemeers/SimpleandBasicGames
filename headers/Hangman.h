#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <ctime>
#include <set>
#include <curl/curl.h>
#include <algorithm>

// using namespace std;

#pragma once
class Hangman
{
private:	
	// list to hold the words we pull (we will add this to our set)
	std::vector<std::string> words =
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

	// use a set to store all words (set to prevent repeats)
	std::set<std::string> wordBank;

	// list to hold guessed letters --> if true, we have guessed, if not, fair game
	std::unordered_map<char, bool> guessed;

	// store the word we are guessing
	std::string currentHangmanWord;

	// store the guessed state of the word
	std::string guessedHangmanWord;

	// number of chances left
	int chances;

	// store the game over state, and if it's a loss/win, and if another game should start
	bool isVictory;
	bool newGame;


	// ---------------------------------------- word bank construction ----------------------------------------
	// Use libcurl to pull data from the API
	// put all of our starting words (that are passed) into the set
	void initWordBank();
	// overall read function for the API itself
	void readContent();
	// parser function to extract the returned data into a usable format
	void parserFunction(std::string rawBuffer);
	// construct the dictionary for hangman
	void wordBankHandler();
	// ---------------------------------------- word bank construction ----------------------------------------


	// ---------------------------------------- game loop -----------------------------------------------------
	
	// initialize functions
	// the instructions given to the player for how to play the game
	void gameExplanation() const;
	// start state --> select word and setup
	void startState();
	
	// main loop
	// guess loop state (guessing for the hangman)
	bool guessingState();

	// end game or start new game
	// end state (victory/loss, quit)
	void endState();

	// --------------------------------------- game state helpers ---------------------------------------------
	// check for already used letters in the answer
	bool hasUsed(char guess);

	bool isValid(char& guess);

	void updateHangman(char guess);

	// "draw" the hangman sentance state
	void drawHangman() const;
	// --------------------------------------- game state helpers ---------------------------------------------
	// --------------------------------------- game loop ------------------------------------------------------


public:
	Hangman();

	// use parser, init the map, etc...
	void hangManInit();

	bool gameLoop();

	void shutdown();

	~Hangman();

};


// Use libcurl to pull data from the API
// function to pass into libcurl handlers (it is used whenever there is data that needs to be saved)
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData);


