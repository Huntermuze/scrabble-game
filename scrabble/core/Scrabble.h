#ifndef ASSIGNMENT_2_NEW_SCRABBLE_H
#define ASSIGNMENT_2_NEW_SCRABBLE_H

#include <fstream>
#include <queue>
#include <utility>
#include <tuple>
#include <map>
#include <ctime>
#include "gameboard/Board.h"
#include "Player.h"
#include "../utility/utils.h"
#include "../utility/LinkedList.h"
#include "../utility/SimpleQueue.h"
#include "../utility/typedefs.h"


class Scrabble {
public:
    // Constructor to create a new game.
    Scrabble(LinkedList<Player> *players, Board *b, const std::string &fileToConsume,
             StringVec *wordsList, bool correctFeature) :
            players(players), wordsList(wordsList), board(b),
            wordCorrectnessFeature(correctFeature) {
        this->tileBag = new LinkedList<Tile>();
        this->playerTurnsOrder = new SimpleQueue<Player>();
        this->letterToPointsMap = new std::map<Letter, Value>();
        createTileBag(fileToConsume);
        shuffleTileBag();
    }

    /*
     * Constructor to load a new game.
     *
     * Handles the file IO, as the serialised game file is unique to this
     * object, and only it should be able to understand and recognise the
     * serialised game file's format.
     */
    Scrabble(const std::string &savedGamePathToConsume, int boardSize, StringVec *wordsList,
             bool correctFeature, bool expansionFeature);

    ~Scrabble();

    // Returns the new tile to pass to the player.
    Tile *replaceTile(Tile *tileToReplace);

    // Begins the game loop.
    void startGame(bool loadedGame);

    // Saves the game to the desired location.
    void saveGame(Player *currPly, const std::string &filename);

private:
    /*
     * Creates the tile bag from a file.
     *
     * The file IO is, once again, handled in this method, because the format
     * of the scrabbletiles.txt is unique to this class and should only be
     * understood by it.
     */
    void createTileBag(const std::string &fileToConsume);

    // Gets amtToGet random tiles from the tilebag, which is given to the player.
    LinkedList<Tile> *getRandomTiles(int amtToGet);

    // Validates the syntax of the commands.
    StringVec *validateCommandArguments(StringVec *args, Player *currPly);

    // Prompts user input if either the syntax or the logic of the command is invalid.
    StringVec *promptInput(Player *currPly);

    // Reads in the player informatioon from the saved game file, and creates the object.
    Player *readInPlayer(std::ifstream &SavedGameFile);

    // Reads in the board information from the saved game file, and creates the object.
    void readInBoard(std::ifstream &SavedGameFile, BoardGrid *boardGrid, int boardSize);

    /*
     * Fills a LinkedList with Tiles from the information presented in the saved
     * game format (i.e., A-1, K-3, L-9, etc).
     */
    void fillLinkedList(LinkedList<Tile> *toFill, std::string &line);

    // Shuffles the tile bag, so that its elements are in random order.
    void shuffleTileBag();

    // Represents the gameplay process and its logic.
    void gameplay();

    // Checks the command's logic, and executes the command.
    void parseCommand(Player *currPly);

    /*
     * Given a list of words in the form of LinkedLists of tiles (not yet converted to a string),
     * this algorithm will convert all the words into a string, and verify them against the
     * official_words_list.txt file (the official scrabble words provided by Ruwan). It will return
     * false if even just one of those words are invalid, and it will also handle the error feedback,
     * such that, it will alert the user that a word (or many) is/are invalid; it will detail the
     * invalid words too.
     */
    bool validateWords(std::vector<LinkedList<Tile> *> *words);

    void displayRoundBeginInfo(Player *currPly);

    void displayAllPlayerScores();

    /*
     * This is a simple accessor method which accesses the internal map which maps letters to their
     * corresponding points amount. It will return the amount of points said letter should give the
     * user.
     */
    int getPointsForLetter(Letter letter);

    void displayGameOverInfo();

    // Check if the location argument is logically valid, and within the constraints requested.
    static bool isLocationValid(const std::string &loc, char maxRowVal, int maxColVal);

    // Check if the letter argument is logically valid, and within the constraints.
    static bool isLetterValid(char letter, char minValidLetter, char maxValidLetter);

    LinkedList<Tile> *tileBag;
    LinkedList<Player> *players;
    /*
     * The official_words_list.txt file read into memory for efficiency purposes (trade-off between
     * speed and memory, wherein we are sacrificing additional memory for additional speed).
     */
    StringVec *wordsList;
    // A Queue holding the references to the two players, emulating their order.
    SimpleQueue<Player> *playerTurnsOrder;
    Board *board;
    std::map<Letter, Value> *letterToPointsMap;
    bool wordCorrectnessFeature;
};


#endif //ASSIGNMENT_2_NEW_SCRABBLE_H
