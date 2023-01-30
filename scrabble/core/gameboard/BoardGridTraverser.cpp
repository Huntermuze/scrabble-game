#include "BoardGridTraverser.h"

BoardGridTraverser::BoardGridTraverser(BoardGrid *boardGrid) {
    this->boardGrid = boardGrid;
}

int BoardGridTraverser::getVerticalUpperBound(int padding) {
    int upperBound = -1;

    for (int i = boardGrid->size() - 1; i >= 0; --i) {
        for (int j = 0; j < boardGrid->size(); ++j) {
            if ((*boardGrid)[i][j].getLetter() != ' ' && i > upperBound) {
                upperBound = i;
            }
        }
    }

    // The board is empty, thus, we reached the end of the boardGrid, and must use the array boundary.
    if (upperBound == -1) {
        upperBound = boardGrid->size() - 1;
    } else {
        upperBound = utils::minimum(upperBound + padding, boardGrid->size() - 1);
    }

    return upperBound;
}

int BoardGridTraverser::getVerticalLowerBound(int padding) {
    int lowerBound = boardGrid->size() - 1;

    for (int i = 0; i < boardGrid->size(); ++i) {
        for (int j = 0; j < boardGrid->size(); ++j) {
            if ((*boardGrid)[i][j].getLetter() != ' ' && i < lowerBound) {
                lowerBound = i;
            }
        }
    }

    // The board is empty, thus, we reached the end of the boardGrid, and must use the array boundary.
    if (lowerBound == boardGrid->size() - 1) {
        lowerBound = 0;
    } else {
        lowerBound = utils::maximum(lowerBound - padding, 0);
    }

    return lowerBound;
}

int BoardGridTraverser::getHorizontalUpperBound(int padding) {
    int upperBound = -1;

    for (int i = 0; i < boardGrid->size(); ++i) {
        for (int j = boardGrid->size() - 1; j >= 0; --j) {
            if ((*boardGrid)[i][j].getLetter() != ' ' && j > upperBound) {
                upperBound = j;
            }
        }
    }

    // The board is empty, thus, we reached the end of the boardGrid, and must use the array boundary.
    if (upperBound == -1) {
        upperBound = boardGrid->size() - 1;
    } else {
        upperBound = utils::minimum(upperBound + padding, boardGrid->size() - 1);
    }

    return upperBound;
}

int BoardGridTraverser::getHorizontalLowerBound(int padding) {
    int lowerBound = boardGrid->size() - 1;

    for (int i = 0; i < boardGrid->size(); ++i) {
        for (int j = 0; j < boardGrid->size(); ++j) {
            if ((*boardGrid)[i][j].getLetter() != ' ' && j < lowerBound) {
                lowerBound = j;
            }
        }
    }

    // The board is empty, thus, we reached the end of the boardGrid, and must use the array boundary.
    if (lowerBound == boardGrid->size() - 1) {
        lowerBound = 0;
    } else {
        lowerBound = utils::maximum(lowerBound - padding, 0);
    }

    return lowerBound;
}
