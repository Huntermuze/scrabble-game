#ifndef ASSIGNMENT_2_BOARD_H
#define ASSIGNMENT_2_BOARD_H

#include <fstream>
#include <utility>
#include "../Tile.h"
#include "../../utility/LinkedList.h"
#include "BoardGridTraverser.h"
#include "../../utility/typedefs.h"
#include "../../utility/utils.h"


class Board : public Serialisable {
public:
    /*
     * Member Initialiser List is both slightly more efficient, and it allows
     * omitting the default constructor specification (less verbose).
     *
     * Create a new Board object with a given sizeDim for the boardGrid.
     */
    Board(int sizeDim, bool expansionFeature) : size(sizeDim),
                                                boardExpansionFeature(expansionFeature) {
        boardGrid = new BoardGrid(size);
        wordsPlaced = new StringVec();
        bgTraverser = new BoardGridTraverser(boardGrid);

        fillBoardGrid();
    };

    /*
     * This Member Initialiser List is designed for loading a game. It is to
     * be used by the Scrabble class.
     *
     * Takes in a pre-existing BoardGrid type, because the grid will be
     * pre-specified and will contain tiles already (likely).
     */
    Board(BoardGrid *board, bool expansionFeature)
            : size(board->size()), boardExpansionFeature(expansionFeature), wordsPlaced(),
              boardGrid(board) {
        bgTraverser = new BoardGridTraverser(board);
    };

    /*
     * Object destructor which overrides the Serialisable destructor, as it is
     * a child class of it.
     */
    ~Board() override;

    /*
     * Places a given tile, at a certain row and column number, irrespective of its validity.
     * isLocationFreeAndValid() should be used prior to this method to ensure the validity of placement.
     */
    void placeTile(const Tile &tileToPlace, char row, int column);

    /*
     * Displays the layout of the grid and its contents to standard output.
     */
    void display();

    StringVec *getWordsPlaced();

    void removeTile(char row, int col);

    void setWordsPlaced(StringVec *newWordsPlaced);

    bool isBoardEmpty();

    bool isLocationFreeAndValid(char row, int column);

    std::vector<LinkedList<Tile> *> *getAllNewWordsOnBoard(StringVec *wordsPlacedNew);

    /*
     * Serialises a given board object into its String representation.
     */
    std::string serialiseToString() override;

    /*
     * Converts the validated user input, which is in the form of a char and int
     * (i.e., A3, K0, L9, etc), into its integer representation, so that it
     * is accessible by the board grid (since it is a 2D vector).
     */
    static std::pair<int, int> logicalCoordsToIndexes(char row, int column);

private:
    /*
     * Initially fills the board grid with empty tiles
     * (i.e., tiles with a dummy letter and value). These are then to be replaced
     * later on during the gameplay. Called during the construction of a Board object.
     */
    void fillBoardGrid();

    /*
     * Since a grid is essentially square, this variable represents the single
     * dimension of that square (could also be called length, width, etc).
     */
    int size;
    bool boardExpansionFeature;
    StringVec *wordsPlaced;
    BoardGridTraverser *bgTraverser;
    // Reference to the underlying BoardGrid (2D vector) object.
    BoardGrid *boardGrid;
};

#endif //ASSIGNMENT_2_BOARD_H
