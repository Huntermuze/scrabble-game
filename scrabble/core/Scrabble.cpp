#include "Scrabble.h"
#include <vector>
#include <algorithm>

Scrabble::~Scrabble() {
    delete this->tileBag;
    delete this->board;
    delete this->playerTurnsOrder;
    delete this->wordsList;
    delete this->letterToPointsMap;
}

void Scrabble::createTileBag(const std::string &fileToConsume) {
    utils::fileExistsElseThrow(fileToConsume);

    std::ifstream TilesFile(fileToConsume);
    std::string line;

    // Read the file and create new Tile objects based on the info.
    while (std::getline(TilesFile, line)) {
        unsigned long len = line.length();

        /*
         * The length of the line should be at least 5 chars long (anything less
         * is invalid input).
         */
        if (len >= 5) {
            auto args = utils::splitString(line, " ");
            Letter l = (*args)[0][0];
            Value val = std::stoi((*args)[1]);
            this->letterToPointsMap->insert(std::pair<Letter, Value>(l, val));

            int quantity = std::stoi((*args)[2]);

            for (int i = 0; i < quantity; ++i) {
                Tile *newTile = new Tile(l, val);
                this->tileBag->add(newTile);
            }
        }
    }

    TilesFile.close();
}

Tile *Scrabble::replaceTile(Tile *tileToReplace) {
    this->tileBag->add(tileToReplace);
    return this->tileBag->pollFirst();
}

void Scrabble::shuffleTileBag() {
    // Set up an array to copy in the tiles from the tileBag, this array wil be what is shuffled
    int tileBagSize = this->tileBag->size();
    Tile *temp[tileBagSize];
    for (int i = 0; i < tileBagSize; i++) {
        temp[i] = this->tileBag->pollFirst();
    }
    // Seed a random number to ensure each compile of the program results in a random tileBag
    srand(time(nullptr));
    // Shuffle the array using the random seed for all tiles in the array
    for (int i = 0; i < tileBagSize; i++) {
        int index = rand() % tileBagSize;
        Tile *rand_tile = temp[i];
        temp[i] = temp[index];
        temp[index] = rand_tile;
    }
    // Return the shuffled tiles into the tileBag, which will now be the tileBag that is
    // initialised when the game starts
    for (int i = 0; i < tileBagSize; i++) {
        this->tileBag->add(temp[i]);
    }
}

void Scrabble::startGame(bool loadedGame) {
    /*
     * If the game is loaded we must clear the buffer, and no need to handle turns
     * or adding tiles to player's hands, as those are already covered.
     */
    if (loadedGame) {
        std::cin.ignore();
    } else {
        players->forEach([this](Player *currPly) {
            playerTurnsOrder->enqueue(currPly);
            currPly->addTilesToHand(getRandomTiles(7));
        });
    }

    gameplay();
}

void Scrabble::displayRoundBeginInfo(Player *currPly) {
    std::cout << std::endl;
    std::cout << currPly->getName() << ", it's your turn" << std::endl;
    displayAllPlayerScores();
    board->display();
    std::cout << std::endl;
    currPly->displayHand();
    std::cout << std::endl;
}

void Scrabble::displayAllPlayerScores() {
    players->forEach([](Player *currPly) {
        std::cout << "Score for " << currPly->getName() << ": " << currPly->getScore() << std::endl;
    });
}

void Scrabble::displayGameOverInfo() {
    std::cout << "Game over" << std::endl;
    displayAllPlayerScores();

    Player *winningPlayer = players->get(0);
    int scoreToCompare = players->get(0)->getScore();
    bool isTie = true;

    for (int i = 0; i < players->size(); ++i) {
        Player *currPly = players->get(i);

        if (currPly->getScore() > winningPlayer->getScore()) {
            winningPlayer = currPly;
        }

        if (currPly->getScore() != scoreToCompare) {
            isTie = false;
        }
    }

    // Ignoring the case where multiple players have the exact same score (not enough time).
    if (isTie) {
        std::cout << "Two or more players tied. Congratulations!" << std::endl;
    } else {
        std::cout << "Player " << winningPlayer->getName() << " won!" << std::endl;
    }
}

void Scrabble::gameplay() {
    bool tileBagEmptyAndHandsEmpty;
    bool gameInProgress;

    do {
        // Get the player's turn.
        Player *currPly = playerTurnsOrder->dequeueThenEnqueue();
        displayRoundBeginInfo(currPly);
        // Validate the syntax, logic of cmd and execute.
        parseCommand(currPly);

        // Check if the tilebag and the hands are empty.
        bool isAHandEmpty = players->checkForEach([](Player *currPly) -> bool {
            return currPly->handSize() == 0;
        });
        tileBagEmptyAndHandsEmpty = this->tileBag->size() == 0 && isAHandEmpty;

        // Check if the game has ended.
        bool hasPlayerPassedTwiceConsecutively = players->checkForEach([](Player *ply) -> bool {
            return ply->hasPassedTwiceConsecutively();
        });
        gameInProgress = !tileBagEmptyAndHandsEmpty && !hasPlayerPassedTwiceConsecutively;

        // If game has not ended refill tiles if necessary and cycle through next turn.
        if (this->tileBag->size() > 7) {
            if (currPly->handSize() < 7 && gameInProgress) {
                currPly->addTilesToHand(getRandomTiles(7 - currPly->handSize()));
            }
        } else {
            if (currPly->handSize() < 7 && gameInProgress) {
                currPly->addTilesToHand(
                        getRandomTiles(this->tileBag->size() - currPly->handSize()));
            }
        }
    } while (gameInProgress);

    displayGameOverInfo();
}

// Will get the correct input from the user.
StringVec *Scrabble::validateCommandArguments(StringVec *args, Player *currPly) {
    // Converted to booleans to prevent nesting. An OOP tree structure would be
    // preferred, but time did not allow it.

    bool cmdStructureValid;
    bool rangeIsValid;

    do {
        bool correctPlaceArgsLen = args->size() == 4;
        bool correctPlaceDoneArgsLen = args->size() == 2;
        bool correctReplaceArgsLen = args->size() == 2;
        bool correctPassArgsLen = args->size() == 1;
        bool correctQuitArgsLen = args->size() == 1;
        bool correctSaveArgsLen = args->size() == 2;

        bool validPlaceCommand = correctPlaceArgsLen && utils::toLower((*args)[0]) == "place" &&
                                 utils::toLower((*args)[2]) == "at";

        bool validPlaceDoneCommand = correctPlaceDoneArgsLen &&
                                     utils::toLower((*args)[0]) == "place" &&
                                     utils::toLower((*args)[1]) == "done";

        bool validReplaceCommand = correctReplaceArgsLen &&
                                   utils::toLower((*args)[0]) == "replace";

        bool validPassCommand =
                correctPassArgsLen && utils::toLower((*args)[0]) == "pass";

        bool validSaveCommand =
                correctSaveArgsLen && utils::toLower((*args)[0]) == "save";

        bool validQuitCommand =
                correctQuitArgsLen && utils::toLower((*args)[0]) == "quit";

        // Relies on C++ short-circuiting to function without error.
        cmdStructureValid = !args->empty() && (validPlaceCommand ||
                                               validPlaceDoneCommand ||
                                               validReplaceCommand ||
                                               validPassCommand ||
                                               validSaveCommand ||
                                               validQuitCommand);

        bool validLetter = true;
        bool validLocation = true;

        if (validPlaceCommand || validReplaceCommand) {
            // Cannot have more, for example, string "AE" as a letter. It must be ONE character.
            if ((*args)[1].length() > 1) {
                validLetter = false;
            } else {
                validLetter = isLetterValid((*args)[1][0], 'A', 'Z');
            }
        }

        if (validPlaceCommand) {
            validLocation = isLocationValid((*args)[3], 'O', 14);
        }

        rangeIsValid = validLetter && validLocation;

        if (!cmdStructureValid) {
            std::cout << "Invalid syntax. You may only enter the following commands:"
                      << std::endl
                      << "1. \"place [letter] at [location]\", where letter = character and location = string (i.e., C3)."
                      << std::endl
                      << "2. \"replace [letter]\", where letter = character."
                      << std::endl
                      << "3. \"pass\"" << std::endl;
            args = promptInput(currPly);
        } else if (!validLetter) {
            std::cout << "That is not a valid letter. Please enter a "
                         "letter between A-O." << std::endl;
            args = promptInput(currPly);
        } else if (!validLocation) {
            std::cout << "That is not a valid location. Please enter a letter "
                         "between A-O, followed by a number between 0-14 "
                         "(i.e., B9 or F0 or M11)" << std::endl;
            args = promptInput(currPly);
        }
    } while (!cmdStructureValid || !rangeIsValid);

    return args;
}

StringVec *Scrabble::promptInput(Player *currPly) {
    std::string command;
    std::cout << "> ";
    std::cin.clear();
    bool isEof = std::getline(std::cin, command).eof();

    if (isEof) {
        std::cout << std::endl;
        /*
         * Save the game if the user quits with EOF, so that they can return to it at anytime.
         * The name of the text file will be suffixed with today's date in dd/mm/YY format.
         */
        const int maxLength = 80;
        char t[80];
        time_t timeNow = time(nullptr);
        strftime(t, maxLength, "%d_%m_%Y", localtime(&timeNow));
        saveGame(currPly, "savedgame_" + std::string(t) + ".txt");
        throw std::runtime_error("Program forcibly quit by user.");
    }

    return utils::splitString(command, " ");
}

void Scrabble::parseCommand(Player *currPly) {
    StringVec *args;
    // Count the number of places done, so that the player can't just immediately
    // jump to using "place done".
    int previousPlaceCmds = 0;

    // A player can have multiple "turns" if they place multiple times.
    bool turnOver = false;
    bool bingoOperation = false;
    auto *tilesToPlace = new std::vector<std::tuple<Tile *, char, int> >();
    auto *wordsPlacedNew = new StringVec();

    while (!turnOver) {
        // Validate command + argument(s) syntax.
        args = validateCommandArguments(promptInput(currPly), currPly);

        // Whatever the command is, it is guaranteed to have valid syntax and
        // to be a valid command.
        std::string baseSyntax = utils::toLower((*args)[0]);

        if (baseSyntax == "place") {
            std::string toPlace = (*args)[1];
            std::string atSyntax;
            std::string location;

            // Safety check to prevent seg fault.
            if (utils::toLower(toPlace) != "done") {
                atSyntax = utils::toLower((*args)[2]);
                location = (*args)[3];
            }

            if (utils::toLower((*args)[1]) == "done") {
                // If they try to place done immediately, disallow.
                if (previousPlaceCmds == 0) {
                    std::cout << "Nice try! You cannot do this unless you have "
                                 "previously placed something. If you wish to "
                                 "skip your turn, please use the \"pass\" command." << std::endl;
                } else {
                    auto words = board->getAllNewWordsOnBoard(wordsPlacedNew);

                    /*
                     * If the word correcting feature is not enabled, then we don't want to validate
                     * the words (only the first boolean value is evaluated in this conditional).
                     */
                    if (!wordCorrectnessFeature || validateWords(words)) {
                        /*
                         * According to the scrabble rules, if two words are made, and they share a tile
                         * then that intersection between the two words is counted once for each word
                         * (i.e., twice).
                         */
                        // Add the points to the player.
                        if (bingoOperation) {
                            std::cout << std::endl << "BINGO!!!" << std::endl;
                            currPly->addPoints(50);
                        }

                        for (int i = 0; i < words->size(); ++i) {
                            for (int j = 0; j < (*words)[i]->size(); ++j) {
                                currPly->addPoints((*words)[i]->get(j)->getValue());
                            }
                        }

                        /*
                         * We must update the wordsPlaced list, so that it now reflects all the
                         * words that are CURRENTLY on the board. This will be useful for the next
                         * turn, when we need to work out what NEW words the player currently placed
                         * (if any), and then calculate the total points earned for these new words.
                         */
                        board->setWordsPlaced(wordsPlacedNew);

                        // End turn if they "place done" validly.
                        turnOver = true;
                    } else {
                        // If any of the words are invalid, we must undo our previous actions.

                        // If the bingo operation was set to true, we should set it back to false.
                        if (bingoOperation) {
                            bingoOperation = false;
                        }

                        // We should add all the tiles taken from the player's hand back to their hand.
                        for (auto tuple: *tilesToPlace) {
                            currPly->addTileToHand(std::get<0>(tuple));
                        }

                        // Remove all the tiles we placed on the board.
                        for (auto tuple: *tilesToPlace) {
                            board->removeTile(std::get<1>(tuple), std::get<2>(tuple));
                        }

                        // Need to reset wordsPlacedNew.
                        wordsPlacedNew->clear();

                        // Need to reset previously placed cmds count.
                        previousPlaceCmds = 0;

                        std::cout << "Please re-enter tiles, so that the placement is valid."
                                  << std::endl;
                        // Display their hand again in-case they forgot their tiles.
                        currPly->displayHand();
                    }
                }
            } else { // Must be a place command (i.e., place X at Y).
                Tile *tileToPlace = currPly->getTileToPlace(toPlace[0]);

                // If the tile was not found in their hand, then it is invalid.
                if (tileToPlace == nullptr) {
                    std::cout << "You do not have a tile with the letter " << toPlace
                              << ". Please choose another from your hand." << std::endl;
                    currPly->displayHand();
                } else {
                    // Place the tile.
                    char row = location[0];
                    // Guaranteed to succeed due to prior validation.
                    int col = std::stoi(location.substr(1, location.length()));
                    /*
                     * If it is the first turn, allow the player to place the tiles anywhere, as there
                     * are no previous tiles to connect it to and everything will be empty. In addition,
                     * if it is the firs turn, and the player tries to play an invalid word, then we
                     * should check if the board is empty (it will be since the placements are undone).
                     * This allows us to ensure that the player can place tiles anywhere on the first
                     * turn, until it ends in a VALID fashion.
                     */
                    bool canPlaceTile =
                            board->isBoardEmpty() || board->isLocationFreeAndValid(row, col);

                    if (canPlaceTile) {
                        /*
                         * Add the tile to the list of tiles to place. This allows us to verify
                         * the word(s) created by the placement, and to undo the placements
                         * in-case of an invalid placement(s).
                         */
                        tilesToPlace->push_back(std::make_tuple(tileToPlace, row, col));
                        board->placeTile(*tileToPlace, row, col);
                        ++previousPlaceCmds;

                        // Special "Bingo" operation.
                        if (previousPlaceCmds == 7) {
                            bingoOperation = true;
                        }
                    } else {
                        std::cout << "You cannot place a tile there!"
                                     " It must both be connected to another word and the location must be empty."
                                  << std::endl;
                        currPly->addTileToHand(tileToPlace);
                    }
                }
            }
        } else if (baseSyntax == "replace" && previousPlaceCmds == 0) {
            // Get the tile to replace from the player's hand.
            Tile *tileToReplace = currPly->getTileToReplace((*args)[1][0]);

            if (tileToReplace == nullptr) {
                std::cout << "You do not have tile " << (*args)[1][0] << " in your hand."
                          << std::endl;
            } else {
                // Give the tile to the tilebag, and get a new tile.
                Tile *newTile = replaceTile(tileToReplace);
                // Give the new tile to the player.
                currPly->addTileToHand(newTile);
                // End the turn.
                turnOver = true;
            }
        } else if (baseSyntax == "pass" && previousPlaceCmds == 0) {
            currPly->pass();
            turnOver = true;
        } else if (baseSyntax == "save") {
            saveGame(currPly, (*args)[1]);
        } else if (baseSyntax == "quit") {
            std::cout << std::endl;
            throw std::runtime_error("Program forcibly quit by user.");
        } else {
            /*
             * Only reason it would end up here is if the user tried to pass/replace
             * after already having placed some tile(s). We can prove this because
             * the input has already been validated, so it must be A valid command.
             * However, if it makes it here, that means the other conditional failed
             * within the check, and the only conditionals with two parts is the
             * replace and pass checks (i.e., previousPlaceCmds == 0).
             */
            std::cout
                    << "You cannot pass or replace if you have already placed tile(s)!"
                       "\nPlease use \"place Done\" instead." << std::endl;
        }
    }
}

/*
 * CONTRACT: The maximum value for row and col is Z and 26 respectively.
 * Otherwise, you run into a different, more complex naming system (i.e., A1B3).
 */
bool Scrabble::isLocationValid(const std::string &loc, char maxRowVal, int maxColVal) {
    int rowValue;

    // Check if the row is an integer.
    try {
        rowValue = std::stoi(loc.substr(1, loc.length()));
    } catch (std::invalid_argument &e) {
        // Force the below validRow boolean to return false.
        rowValue = -1;
    }

    // Check if the row is an ASCII char between A-maxRowVal.
    bool validRow = isLetterValid(loc[0], 'A', maxRowVal);
    bool validCol = rowValue >= 0 && rowValue <= maxColVal;

    return validCol && validRow;
}

bool Scrabble::isLetterValid(char letter, char minValidLetter, char maxValidLetter) {
    return letter >= minValidLetter && letter <= maxValidLetter;
}

LinkedList<Tile> *Scrabble::getRandomTiles(int amtToGet) {
    auto *newHand = new LinkedList<Tile>();

    // Take out amtToGet tiles. However, if we have run out, do not try to take out anymore.
    // The game will end after this.
    for (int i = 0; i < amtToGet && this->tileBag->size() != 0; ++i) {
        Tile *tile = this->tileBag->pollFirst();
        newHand->add(tile);
    }

    return newHand;
}

Player *Scrabble::readInPlayer(std::ifstream &SavedGameFile) {
    std::string playerName;
    int playerScore;
    auto *playerTiles = new LinkedList<Tile>();
    std::string line;

    std::getline(SavedGameFile, line);
    playerName = line;

    std::getline(SavedGameFile, line);
    playerScore = std::stoi(line);

    std::getline(SavedGameFile, line);
    fillLinkedList(playerTiles, line);

    return new Player(playerName, playerTiles, playerScore);
}

void Scrabble::readInBoard(std::ifstream &SavedGameFile, BoardGrid *boardGrid, int boardSize) {
    std::string line;

    for (int lineCount = 0; lineCount < 17; ++lineCount) {
        std::getline(SavedGameFile, line);

        // Skip the first two header lines.
        if (lineCount > 1) {
            std::vector<std::string> *curBoardLine = utils::splitString(line, "|");

            // Remove first column identifier.
            (*curBoardLine).erase((*curBoardLine).begin());
            // Remove the last element, as it is split as an empty string since it
            // contains a pipe "|".
            (*curBoardLine).pop_back();

            // Fill the 2D boardGrid.
            (*boardGrid)[lineCount - 2] = std::vector<Tile>(boardSize);

            for (std::size_t j = 0;
                 j < (*curBoardLine).size(); ++j) {
                (*curBoardLine)[j] = (*curBoardLine)[j].substr(1, (*curBoardLine)[j].length() - 1);
                Letter l = (*curBoardLine)[j][0];
                Value val = l == ' ' ? -1 : this->getPointsForLetter(l);

                (*boardGrid)[lineCount - 2][j] = *new Tile(l, val);
            }
        }
    }
}

void Scrabble::fillLinkedList(LinkedList<Tile> *toFill, std::string &line) {
    std::vector<std::string> *savedTileBag = utils::splitString(line, ", ");
    for (auto &i: *savedTileBag) {
        std::vector<std::string> *pair = utils::splitString(i, "-");
        Letter l = (*pair)[0][0];
        Value val = std::stoi((*pair)[1]);
        toFill->add(new Tile(l, val));

        delete pair;
    }
}

Scrabble::Scrabble(const std::string &savedGamePathToConsume, int boardSize, StringVec *wordsList,
                   bool correctFeature, bool expansionFeature) {
    utils::fileExistsElseThrow(savedGamePathToConsume);

    auto *boardGrid = new BoardGrid(boardSize);

    // Set fields.
    this->tileBag = new LinkedList<Tile>();
    this->players = new LinkedList<Player>();
    this->wordsList = wordsList;
    this->playerTurnsOrder = new SimpleQueue<Player>();
    this->board = new Board(boardGrid, expansionFeature);
    this->letterToPointsMap = new std::map<Letter, Value>();
    this->wordCorrectnessFeature = correctFeature;

    // Load file.
    std::ifstream SavedGameFile(savedGamePathToConsume);
    std::string line;

    std::vector<std::vector<std::string> > boardGridAsStrings;


    std::getline(SavedGameFile, line);
    int numPlayers = std::stoi(line);
    // Read in the player objects.
    for (int i = 0; i < numPlayers; ++i) {
        this->players->add(readInPlayer(SavedGameFile));
    }

    // Read in the letter->points mapping and create the map.
    for (int i = 0; i < 26; ++i) {
        std::getline(SavedGameFile, line);
        auto mapping = utils::splitString(line, " ");
        Letter l = (*mapping)[0][0];
        Value val = std::stoi((*mapping)[1]);

        this->letterToPointsMap->insert(std::pair<Letter, Value>(l, val));
    }

    // Read in the board contents and create an object.
    readInBoard(SavedGameFile, boardGrid, boardSize);

    // Read in the tilebag and set up the tileBag object.
    std::getline(SavedGameFile, line);
    fillLinkedList(this->tileBag, line);

    // Determine the ordering by queueing the first, second, third & fourth respectively (if applicable).
    for (int i = 0; i < numPlayers; ++i) {
        // Read in the player up next.
        std::getline(SavedGameFile, line);

        Player *plyToQueue = players->getIf(
                [line](Player *ply) -> bool { return ply->getName() == line; });

        this->playerTurnsOrder->enqueue(plyToQueue);
    }

    auto *newWordsPlaced = new StringVec();
    while (!std::getline(SavedGameFile, line).eof()) {
        newWordsPlaced->push_back(line);
    }
    this->board->setWordsPlaced(newWordsPlaced);

    // Safely close file.
    SavedGameFile.close();
}


void Scrabble::saveGame(Player *currPly, const std::string &filename) {
    // Needs to be saved in specific order
    std::ofstream SaveDestination(filename);

    SaveDestination << players->size() << std::endl;
    // Serialise all the constituent objects.
    // Serialise all player objects.
    for (int i = 0; i < players->size(); ++i) {
        SaveDestination << players->get(i)->serialiseToString() << std::endl;
    }

    // Serialise the letter to points mapping.
    std::map<Letter, Value>::iterator itr;
    for (itr = letterToPointsMap->begin(); itr != letterToPointsMap->end(); ++itr) {
        SaveDestination << itr->first << " " << itr->second << std::endl;
    }

    // Serialise the board and all its tiles.
    SaveDestination << board->serialiseToString();
    // Serialise the tilebag and all its tiles.
    SaveDestination << tileBag->serialiseToString() << std::endl;
    // Serialise the player order starting with the player up next.
    SaveDestination << currPly->getName() << std::endl;
    // Exclude the last player on list, as they are actually the first player up [dequeueThenEnqueue()].
    for (int i = 0; i < playerTurnsOrder->size() - 1; ++i) {
        SaveDestination << (*playerTurnsOrder)[i]->getName() << std::endl;
    }

    // Serialise the words placed, to ensure correct functionality upon the next load.
    for (auto &word: *board->getWordsPlaced()) {
        SaveDestination << word << std::endl;
    }

    SaveDestination.close();
}

int Scrabble::getPointsForLetter(Letter letter) {
    return (*letterToPointsMap)[letter];
}

bool Scrabble::validateWords(std::vector<LinkedList<Tile> *> *words) {
    bool validWord = true;

    for (auto word: *words) {
        std::string currWord;

        // Build the word first.
        for (int i = 0; i < word->size(); ++i) {
            currWord += word->get(i)->getLetter();
        }

        /*
         * Convert the word to its lowercase form, and then attempt to find the word in the
         * in-memory official_words_list.txt file version.
         */
        std::string lCurrWord = utils::toLower(currWord);
        int wordInd = utils::getIndexOfWord(wordsList, lCurrWord);

        // If the word is not present in this list, then it is not a valid scrabble word.
        if ((*wordsList)[wordInd] != lCurrWord) {
            validWord = false;
            // Notify the user exactly which word(s) is/are invalid.
            std::cout << currWord << " is not a valid word!" << std::endl;
        }
    }

    return validWord;
}
