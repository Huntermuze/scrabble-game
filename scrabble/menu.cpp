#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include "core/Scrabble.h"

#define EXIT_SUCCESS    0

// Prints out the main menu options.
void printMainMenu(bool boardExpansion, bool wordValidation);

// Instantiates a new Scrabble object and begins the game.
void newGame(const std::string &wordsListPath, bool boardExpansion, bool wordValidation);

/*
 * Instantiates a new Scrabble object, but with pre-defined contents, and starts
 * the game.
 */
void loadGame(const std::string &wordsListPath, bool boardExpansion, bool wordValidation);

// Displays the credits.
void credits();

// Executes the program shutdown functionality.
void gameQuit();

// Sets up a game.
void setupGame(Scrabble *game, bool loadedGame);

// Gets a player's name and performs input validation.
std::string getPlayerName(int playerNum);

// Validates the given player name, enforcing the capitalisation requirement.
bool validatePlayerName(const std::string &nameToValidate);

// Read in the official_words_list.txt file and store it in memory for fast access.
StringVec *readInWordsList(const std::string &path);

// Validates and gets the number of players from the standard input.
int getValidAmountOfPlayers();

int main(int argc, char **argv) {
    bool boardExpansion = false;
    bool wordValidation = false;
    bool invalidInput = false;

    // Validate the commandline arguments.
    if (argc == 3) {
        std::string arg1 = std::string(argv[1]);
        std::string arg2 = std::string(argv[2]);

        if ((arg1 == "-wordvalidation" || arg1 == "-boardexpansion") &&
            (arg2 == "-wordvalidation" || arg2 == "-boardexpansion")) {
            boardExpansion = true;
            wordValidation = true;
        } else {
            invalidInput = true;
        }
    } else if (argc == 2) {
        std::string arg1 = std::string(argv[1]);
        if (arg1 == "-wordvalidation") {
            wordValidation = true;
        } else if (arg1 == "-boardexpansion") {
            boardExpansion = true;
        } else {
            invalidInput = true;
        }
    }

    // If the commandline arguments are invalid, notify the user.
    if (invalidInput) {
        std::cout << "Incorrect command-line arguments. You may choose from the "
                     "following (in any order):\n* [-wordvalidation]\n* [-boardexpansion]"
                  << std::endl;
    } else {
        std::cout << "          Welcome to Scrabble!" << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        // Notify the user about which enhancements have been enabled/disabled.
        if (wordValidation) {
            std::cout << "[ENHANCEMENT] > Word Validation enabled!" << std::endl;
        } else {
            std::cout << "[ENHANCEMENT] > Word Validation disabled!" << std::endl;
        }

        if (boardExpansion) {
            std::cout << "[ENHANCEMENT] > Board Expansion enabled!" << std::endl;
        } else {
            std::cout << "[ENHANCEMENT] > Board Expansion disabled!" << std::endl;
        }
        std::cout << "--------------------------------------" << std::endl << std::endl;

        printMainMenu(boardExpansion, wordValidation);
    }
    return EXIT_SUCCESS;
}


void printMainMenu(bool boardExpansion, bool wordValidation) {
    std::string wordsListPath = "../resources/official_words_list.txt";
    std::string selection;
    int menuSelection;
    bool exit = false;

    std::cout << "Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New game" << std::endl;
    std::cout << "2. Load game" << std::endl;
    std::cout << "3. Credits (Show student information)" << std::endl;
    std::cout << "4. Quit" << std::endl;

    while (!exit) {
        std::cout << "> ";
        std::cin >> selection;
        std::cin.ignore();

        if (std::cin.eof()) {
            std::cout << std::endl;
            gameQuit();
            exit = true;
        } else {
            // Type check.
            try {
                menuSelection = std::stoi(selection);

                // Range check.
                if (menuSelection < 1 || menuSelection > 4) {
                    std::cout << "Your choice must be between 1-4 (bounds included)." << std::endl;
                } else {
                    if (menuSelection == 1) {
                        newGame(wordsListPath, boardExpansion, wordValidation);
                    } else if (menuSelection == 2) {
                        loadGame(wordsListPath, boardExpansion, wordValidation);
                    } else if (menuSelection == 3) {
                        credits();
                    }

                    if (menuSelection != 3) {
                        gameQuit();
                        exit = true;
                    }
                }
            } catch (std::invalid_argument &e) {
                std::cout << "Invalid option. Please choose either 1, 2, 3, or 4." << std::endl;
            } catch (std::runtime_error &e) {
                std::cout << e.what() << std::endl;
                gameQuit();
                exit = true;
            }
        }
    }
}

void newGame(const std::string &wordsListPath, bool boardExpansion, bool wordValidation) {
    std::cout << std::endl;
    std::cout << "Starting a new game" << std::endl;
    std::cout << "Please enter the number of players (2-4):" << std::endl;

    int numPlayers = getValidAmountOfPlayers();
    auto *players = new LinkedList<Player>();


    for (int i = 1; i <= numPlayers; ++i) {
        std::string plyName = getPlayerName(i);
        Player *ply = new Player(std::move(plyName));
        players->add(ply);
    }

    Board *board = new Board(15, boardExpansion);
    std::string tilesFile = "../resources/scrabbletiles.txt";
    Scrabble *game = new Scrabble(players, board, tilesFile,
                                  readInWordsList(wordsListPath),
                                  wordValidation);

    setupGame(game, false);
}

void loadGame(const std::string &wordsListPath, bool boardExpansion, bool wordValidation) {
    std::cout << std::endl;
    std::cout << "Enter the filename of the game to load:" << std::endl;

    Scrabble *game;
    bool fileInvalid;

    do {
        std::cout << "> ";
        std::string filePath;
        std::cin >> filePath;

        if (std::cin.eof()) {
            std::cout << std::endl;
            throw std::runtime_error("Program forcibly quit by user.");
        }

        try {
            game = new Scrabble(filePath, 15,
                                readInWordsList(wordsListPath), wordValidation,
                                boardExpansion);
            fileInvalid = false;
        } catch (std::runtime_error &fileNotFound) {
            fileInvalid = true;
        }

        if (fileInvalid) {
            std::cout << "\"" << filePath << "\"" << " is not a valid file path!" << std::endl;
        }

        std::cout << std::endl;
    } while (fileInvalid);

    std::cout << std::endl;
    std::cout << "Scrabble game successfully loaded!" << std::endl;
    setupGame(game, true);
}


void credits() {
    std::cout << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Name: Matthew Yamen" << std::endl;
    std::cout << "Student ID: s3902219" << std::endl;
    std::cout << "Email: s3902219@student.rmit.edu.au" << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void gameQuit() {
    std::cout << std::endl;
    std::cout << "Goodbye :)" << std::endl;
}

void setupGame(Scrabble *game, bool loadedGame) {
    try {
        std::cout << "Let's play!" << std::endl;
        game->startGame(loadedGame);
    } catch (std::runtime_error &e) {}
}

std::string getPlayerName(int playerNum) {
    std::string playerName;
    std::cout << "Enter a name for player " << playerNum << " (uppercase characters only):"
              << std::endl;

    do {
        std::cout << "> ";
        std::getline(std::cin, playerName);

        if (std::cin.eof()) {
            std::cout << std::endl;
            throw std::runtime_error("Program forcibly quit by user.");
        }

        std::cout << std::endl;
    } while (!validatePlayerName(playerName));

    return playerName;
}

bool validatePlayerName(const std::string &nameToValidate) {
    bool valid = true;

    if (nameToValidate.length() == 0) {
        valid = false;
    }

    for (char c: nameToValidate) {
        if (c < 'A' || c > 'Z') {
            valid = false;
        }
    }

    if (!valid) {
        std::cout
                << "Invalid player name. You must enter a name that is entirely capitalised."
                << std::endl;
    }

    return valid;
}

int getValidAmountOfPlayers() {
    int numPlayers = -1;
    while (numPlayers == -1) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (std::cin.eof()) {
            std::cout << std::endl;
            throw std::runtime_error("Program forcibly quit by user.");
        }

        try {
            int unvalidatedNumPlayers = std::stoi(input);

            if (unvalidatedNumPlayers >= 2 && unvalidatedNumPlayers <= 4) {
                numPlayers = unvalidatedNumPlayers;
            } else {
                std::cout
                        << "You must enter a number between 2 and 4 (inclusive)!"
                        << std::endl;
            }

        } catch (std::invalid_argument &e) {
            std::cout << "You must enter a number!"
                      << std::endl;
        }
        std::cout << std::endl;
    }

    return numPlayers;
}

StringVec *readInWordsList(const std::string &path) {
    utils::fileExistsElseThrow(path);
    // Load file.
    std::ifstream WordsList(path);
    std::string line;

    StringVec *wordsList = new StringVec();

    while (std::getline(WordsList, line)) {
        wordsList->push_back(line);
    }
    WordsList.close();

    return wordsList;
}