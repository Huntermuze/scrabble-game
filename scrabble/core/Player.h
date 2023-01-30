#ifndef ASSIGNMENT_2_PLAYER_H
#define ASSIGNMENT_2_PLAYER_H

#include <iostream>
#include <utility>
#include <vector>
#include "../utility/LinkedList.h"
#include "Tile.h"
#include "../utility/typedefs.h"

class Player : public Serialisable {
public:
    explicit Player(std::string name) : name(std::move(name)) {
        this->hand = new LinkedList<Tile>();
        this->historyOfTurns = new StringVec();
        this->score = 0;
    };

    Player(std::string name, LinkedList<Tile> *hand)
            : name(std::move(name)), hand(hand) {
        this->historyOfTurns = new StringVec();
        this->score = 0;
    };

    // For loading a game (everything should be passed, except turn history).
    Player(std::string name, LinkedList<Tile> *hand, int score)
            : name(std::move(name)), score(score), hand(hand) {
        this->historyOfTurns = new StringVec();
    };

    ~Player() override;

    std::string getName();

    int getScore() const;

    int handSize();

    // Add points to the player's score.
    void addPoints(int pointsToAdd);

    // Deduct points to the player's score.
    void deductPoints(int pointsToDeduct);

    void addTileToHand(Tile *tileToAdd);

    void addTilesToHand(LinkedList<Tile> *tilesToAdd);

    // Adds to historyOfTurns list to allow for passing twice checks.
    void pass();

    // Checks if the player has passed twice in a row.
    bool hasPassedTwiceConsecutively();

    // Print out the player's hand.
    void displayHand();

    // Serialises the object to its string representation.
    std::string serialiseToString() override;

    /*
     * Gets the tile to place, so that it can be given to the  board.
     *
     * Returns nullptr if the player does not have the desired letter in their
     * hand, else it returns the tile and removes it from their hand.
     */
    Tile *getTileToPlace(char letterOfTileToPlace);

    /*
     * Gets the tile to replace, so that it can be given to Scrabble's tileBag.
     *
     * Returns nullptr if the player does not have the desired letter in their
     * hand, else it returns the tile and removes it from their hand.
     */
    Tile *getTileToReplace(char letterOfTileToReplace);

private:
    Tile *getTileFromHand(char letterOfTile, const std::string &history);

    std::string name;
    int score;
    LinkedList<Tile> *hand;
    StringVec* historyOfTurns;
};

#endif //ASSIGNMENT_2_PLAYER_H
