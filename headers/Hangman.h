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

//using namespace std;

#pragma once
class Hangman
{
private:	
	// list to hold the words we pull (we will add this to our set)
	std::vector<std::string> m_words;
	// don't put information in the header file

	// use a set to store all words (set to prevent repeats)
	std::set<std::string> m_wordBank;

	// list to hold guessed letters --> if true, we have guessed, if not, fair game
	std::unordered_map<char, bool> m_guessed;

	// store the word we are guessing
	std::string m_currentHangmanWord;

	// store the guessed state of the word
	std::string m_guessedHangmanWord;

	// number of chances left
	int m_chances;

	// store the game over state, and if it's a loss/win, and if another game should start
	bool m_isVictory;
	bool m_newGame;


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

	// helpers
	// check for already used letters in the answer
	const bool hasUsed(char guess) const;

	const bool isValid(char& guess) const;

	void updateHangman(char guess);

	// "draw" the hangman sentance state
	void drawHangman() const;
	


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



