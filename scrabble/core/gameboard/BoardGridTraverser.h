#ifndef ASSIGNMENT_2_NEW_BOARDGRIDTRAVERSER_H
#define ASSIGNMENT_2_NEW_BOARDGRIDTRAVERSER_H

#include "../../utility/typedefs.h"
#include "../../utility/utils.h"

class BoardGridTraverser {
public:
    explicit BoardGridTraverser(BoardGrid *boardGrid);

    /*
     * Find and retrieve the vertical upper bound (i.e., the biggest row value that is furthest
     * from zero), such that this bound represents the furthest (a non-empty tile) was placed
     * from 0 (row-wise) PLUS the amount of padding (in this assignment, it will be 1, as mentioned
     * in the spec).
     *
     * The value will be clamped if it is greater than the maximum row number (in this assignment,
     * it is 14).
     */
    int getVerticalUpperBound(int padding);

    /*
     * Find and retrieve the vertical lower bound (i.e., the smallest row value that is closest
     * to zero), such that this bound represents the closest (a non-empty tile) was placed
     * from 0 (row-wise) PLUS the amount of padding (in this assignment, it will be 1, as mentioned
     * in the spec).
     *
     * The value will be clamped if it is less than the minimum row number (will always be 0).
     */
    int getVerticalLowerBound(int padding);

    /*
     * Find and retrieve the horizontal upper bound (i.e., the biggest column value that is furthest
     * from zero), such that this bound represents the furthest (a non-empty tile) was placed
     * from 0 (column-wise) PLUS the amount of padding (in this assignment, it will be 1, as mentioned
     * in the spec).
     *
     * The value will be clamped if it is greater than the maximum column number (in this assignment,
     * it is 14).
     */
    int getHorizontalUpperBound(int padding);

    /*
     * Find and retrieve the horizontal lower bound (i.e., the smallest column value that is closest
     * to zero), such that this bound represents the closest (a non-empty tile) was placed
     * from 0 (column-wise) PLUS the amount of padding (in this assignment, it will be 1, as mentioned
     * in the spec).
     *
     * The value will be clamped if it is less than the minimum column number (will always be 0).
     */
    int getHorizontalLowerBound(int padding);

private:
    // We need access to the boardgrid to calculate these figures.
    BoardGrid *boardGrid;
};


#endif //ASSIGNMENT_2_NEW_BOARDGRIDTRAVERSER_H
