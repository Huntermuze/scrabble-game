#include "Board.h"

Board::~Board() {
    // Loop through all the Tile objects and destroy them.
    for (std::size_t i = 0; i < boardGrid->size(); ++i) {
        for (std::size_t j = 0; j < boardGrid->size(); ++j) {
            // Destroy tile objects.
            delete &(*boardGrid)[i][j];
        }
        // Destroy inner vector housing the Tile objects.
        delete &(*boardGrid)[i];
    }
}

bool Board::isBoardEmpty() {
    bool empty = true;

    for (std::size_t i = 0; i < boardGrid->size(); ++i) {
        for (std::size_t j = 0; j < boardGrid->size(); ++j) {
            if ((*boardGrid)[i][j].getLetter() != ' ') {
                empty = false;
            }
        }
    }

    return empty;
}

void Board::placeTile(const Tile &tileToPlace, char row, int column) {
    // Transform the coordinates into accessible ints for the board grid.
    std::pair<int, int> rowCol = logicalCoordsToIndexes(row, column);
    (*this->boardGrid)[rowCol.first][rowCol.second] = tileToPlace;
}

bool Board::isLocationFreeAndValid(char row, int column) {
    // Transform the coordinates into accessible ints for the board grid.
    std::pair<int, int> rowCol = logicalCoordsToIndexes(row, column);

    /*
     * Check if the coordinates point to a tile whose value is -1 (the dummy
     * value indicating that the grid location is free).
     */
    bool posAvailable = (*this->boardGrid)[rowCol.first][rowCol.second].getLetter() == ' ';

    // We must not check outside the bounds of the array, or we will access a random chunk of memory.
    int left = utils::maximum(rowCol.second - 1, 0);
    int right = utils::minimum(rowCol.second + 1, size - 1);
    int up = utils::maximum(rowCol.first - 1, 0);
    int down = utils::minimum(rowCol.first + 1, size - 1);

    // We must also check if there are tiles around this tile (i.e., if it is connected).
    bool leftConnection = (*this->boardGrid)[rowCol.first][left].getLetter() != ' ';
    bool rightConnection = (*this->boardGrid)[rowCol.first][right].getLetter() != ' ';
    bool upConnection = (*this->boardGrid)[up][rowCol.second].getLetter() != ' ';
    bool downConnection = (*this->boardGrid)[down][rowCol.second].getLetter() != ' ';

    bool isConnected = leftConnection || rightConnection || upConnection || downConnection;

    return posAvailable && isConnected;
}


std::pair<int, int> Board::logicalCoordsToIndexes(char row, int column) {
    /*
     * The base10 ASCII code for 'A' is 65. Hence, the difference between A and
     * the row must be equivalent to the desired row number
     * (i.e., C - A = 67 - 65 = 2, which logically maps to row 'C').
     */
    int rowAsInt = row - 65;

    // Return an std::pair object.
    return {rowAsInt, column};
}

void Board::display() {
    int horizontalLowerBound = 0;
    int horizontalUpperBound = size - 1;
    int verticalUpperBound = size - 1;
    int verticalLowerBound = 0;

    /*
     * If the board expansion feature is enabled, we must find the four boundaries (essentially,
     * the left, right, top and bottom boundaries), such that anything within these boundaries, plus
     * an additional column/row in each boundary, is shown, and the rest is hidden from the user.
     */
    if (boardExpansionFeature) {
        horizontalLowerBound = bgTraverser->getHorizontalLowerBound(1);
        horizontalUpperBound = bgTraverser->getHorizontalUpperBound(1);
        verticalUpperBound = bgTraverser->getVerticalUpperBound(1);
        verticalLowerBound = bgTraverser->getVerticalLowerBound(1);
    }

    // Draw the two headers.
    for (int m = 0; m < 2; ++m) {
        std::cout << "  ";
        for (int n = horizontalLowerBound; n <= horizontalUpperBound; ++n) {
            // If drawing first header (the numbers), we need spaces between them.
            if (m == 0) {
                /*
                 * For single digits, we can do 2 spaces, but for double digits
                 * we can only have one space or the placement will be off.
                 */
                if (n > 9) {
                    std::cout << " " << n << " ";
                } else {
                    std::cout << "  " << n << " ";
                }
            } else {
                // Draw the second header which is a long underline/underscore.
                std::cout << "----";

                // Add a dash so that the top table underline is equal.
                if (n == horizontalUpperBound) {
                    std::cout << "-";
                }
            }
        }
        std::cout << std::endl;
    }

    /*
     * Simply loop through the boardGrid and print the letters punctuated with
     * '|' and spaces ' ', where appropriate.
     */
    for (int i = verticalLowerBound; i <= verticalUpperBound; ++i) {
        for (int j = horizontalLowerBound; j <= horizontalUpperBound; ++j) {
            if (j == horizontalLowerBound) {
                std::cout << std::string(1, 'A' + i) << " ";
            }
            std::cout << "| " << (*boardGrid)[i][j].getLetter() << " ";
        }
        std::cout << "|" << std::endl;
    }
}

std::string Board::serialiseToString() {
    std::string serialisedBoard;

    /*
     * Repeat the same process as the display() function, except, instead of the
     * output being standard output, it is to the serialisedBoard string, which
     * will then be output.
     */
    for (int m = 0; m < 2; ++m) {
        serialisedBoard.append("  ");
        for (int n = 0; n < size; ++n) {
            if (m == 0) {
                if (n > 9) {
                    serialisedBoard.append(" ").append(
                            std::to_string(n)).append(" ");
                } else {
                    serialisedBoard.append("  ").append(
                            std::to_string(n)).append(" ");
                }
            } else {
                serialisedBoard.append("----");

                if (n == size - 1) {
                    serialisedBoard.append("-");
                }
            }
        }
        serialisedBoard.append("\n");
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j == 0) {
                serialisedBoard.append(std::string(1, 'A' + i)).append(" ");
            }
            serialisedBoard.append("| ").append(
                    std::string(1, (*boardGrid)[i][j].getLetter())).append(" ");
        }
        serialisedBoard.append("|").append("\n");
    }

    return serialisedBoard;
}

void Board::fillBoardGrid() {
    /*
     * Loop through the BoardGrid object and fill it with empty Tile objects
     * (i.e., tiles with dummy values for letter and value fields).
     */
    for (std::size_t i = 0; i < boardGrid->size(); ++i) {
        (*boardGrid)[i] = std::vector<Tile>(size);
        for (std::size_t j = 0; j < (*boardGrid)[0].size(); ++j) {
            (*boardGrid)[i][j] = *new Tile();
        }
    }
}

StringVec *Board::getWordsPlaced() {
    return this->wordsPlaced;
}

std::vector<LinkedList<Tile> *> *Board::getAllNewWordsOnBoard(StringVec *wordsPlacedNew) {
    auto words = std::vector<LinkedList<Tile> *>();
    auto newWords = new std::vector<LinkedList<Tile> *>();

    // Explore vertically first.
    for (int col = 0; col < (*boardGrid)[0].size(); ++col) {
        auto wordToAdd = new LinkedList<Tile>();

        for (int row = 0; row < boardGrid->size(); ++row) {
            auto curr = (*boardGrid)[row][col];
            if (curr.getLetter() != ' ') {
                wordToAdd->add(new Tile(curr));
            }
        }

        if (wordToAdd->size() <= 1) {
            delete wordToAdd;
        } else {
            words.push_back(wordToAdd);
        }
    }

    // Explore horizontally next.
    for (int row = 0; row < (*boardGrid)[0].size(); ++row) {
        auto wordToAdd = new LinkedList<Tile>();

        for (int col = 0; col < boardGrid->size(); ++col) {
            auto curr = (*boardGrid)[row][col];
            if (curr.getLetter() != ' ') {
                wordToAdd->add(new Tile(curr));
            }
        }

        if (wordToAdd->size() <= 1) {
            delete wordToAdd;
        } else {
            words.push_back(wordToAdd);
        }
    }


    for (auto word: words) {
        std::string currWord;

        // Build the word first.
        for (int i = 0; i < word->size(); ++i) {
            currWord += word->get(i)->getLetter();
        }

        wordsPlacedNew->push_back(currWord);
        bool newWord = true;

        /*
         * I am aware that the downfall to this method is duplicate words, and I would fix it if
         * I had time, however, time is not on my side right now, and this is the best I could do
         * given my circumstances.
         */
        for (auto &i: *wordsPlaced) {
            if (i == currWord) {
                newWord = false;
            }
        }

        // Add it to the new words list.
        if (newWord) {
            newWords->push_back(word);
        }
    }

    return newWords;
}

void Board::setWordsPlaced(StringVec *newWordsPlaced) {
    delete wordsPlaced;
    this->wordsPlaced = newWordsPlaced;
}

void Board::removeTile(char row, int col) {
    std::pair<int, int> rowCol = logicalCoordsToIndexes(row, col);
    (*boardGrid)[rowCol.first][rowCol.second] = Tile();
}
