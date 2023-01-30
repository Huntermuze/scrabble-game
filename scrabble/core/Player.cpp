#include "Player.h"

Player::~Player() {
    delete this->hand;
    delete this->historyOfTurns;
}

std::string Player::getName() {
    return this->name;
}

int Player::getScore() const {
    return this->score;
}

void Player::addPoints(int pointsToAdd) {
    this->score += pointsToAdd;
}

void Player::deductPoints(int pointsToDeduct) {
    this->score -= pointsToDeduct;
}

Tile *Player::getTileToPlace(char letterOfTileToPlace) {
    return this->getTileFromHand(letterOfTileToPlace, "place");
}

Tile *Player::getTileToReplace(char letterOfTileToReplace) {
    return this->getTileFromHand(letterOfTileToReplace, "replace");
}

/*
 * Given that all letter types share the same points, then we only need to find the first letter
 * in the player's hand that has the same letter (i.e., all A's will be associated with 1 point).
 */
Tile *Player::getTileFromHand(char letterOfTile, const std::string &history) {
    Node<Tile> *nodeFound = nullptr;
    Tile *tileToReturn = nullptr;

    for (int i = 0; i < this->hand->size(); ++i) {
        Node<Tile> *currNode = (*this->hand)[i];
        if (currNode->getValue()->getLetter() == letterOfTile) {
            nodeFound = currNode;
        }
    }

    if (nodeFound != nullptr) {
        tileToReturn = new Tile(*nodeFound->getValue());
        hand->removeNode(nodeFound);
        this->historyOfTurns->push_back(history);
    }

    return tileToReturn;
}


void Player::addTilesToHand(LinkedList<Tile> *tilesToAdd) {
    for (int i = 0; i < tilesToAdd->size(); ++i) {
        Tile *newTile = new Tile(*tilesToAdd->get(i));
        this->hand->add(newTile);
    }

    delete tilesToAdd;
}

void Player::addTileToHand(Tile *tileToAdd) {
    this->hand->add(tileToAdd);
}

void Player::displayHand() {
    std::cout << "Your hand is:" << std::endl;

    for (int i = 0; i < this->hand->size(); ++i) {
        Tile *t = (*this->hand)[i]->getValue();
        if (i == this->hand->size() - 1) {
            std::cout << t->getLetter() << "-" << t->getValue();
        } else {
            std::cout << t->getLetter() << "-" << t->getValue() << ", ";
        }
    }

    std::cout << std::endl;
}

std::string Player::serialiseToString() {
    std::string serialisedPlayer;
    serialisedPlayer.append(this->getName()).append("\n");
    serialisedPlayer.append(std::to_string(this->getScore())).append("\n");
    serialisedPlayer.append(this->hand->serialiseToString());

    return serialisedPlayer;
}

int Player::handSize() {
    return this->hand->size();
}

void Player::pass() {
    this->historyOfTurns->push_back("pass");
}

bool Player::hasPassedTwiceConsecutively() {
    bool passedTwice = false;

    // Must check if the player has had at least two turns, to prevent invalid access.
    if (historyOfTurns->size() > 1) {
        for (std::size_t i = 0; i < historyOfTurns->size() - 1; ++i) {
            if ((*historyOfTurns)[i] == "pass" && (*historyOfTurns)[i + 1] == "pass") {
                passedTwice = true;
            }
        }
    }
    return passedTwice;
}


