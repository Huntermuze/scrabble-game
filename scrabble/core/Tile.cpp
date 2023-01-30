#include "Tile.h"

Tile::Tile(const Tile &other) {
    this->letter = other.getLetter();
    this->value = other.getValue();
}

Tile::Tile(Letter letter, Value val) {
    this->letter = letter;
    this->value = val;
}

Tile::Tile() {
    this->letter = ' ';
    this->value = -1;
}

Letter Tile::getLetter() const {
    return letter;
}

Value Tile::getValue() const {
    return value;
}

void Tile::setValue(Value newValue) {
    this->value = newValue;
}

void Tile::setLetter(Letter newLetter) {
    this->letter = newLetter;
}

bool Tile::operator==(const Tile &other) const {
    return other.letter == this->letter && other.value == this->value
           && typeid(other) == typeid(*this);
}

std::string Tile::serialiseToString() {
    std::string serialisedTile;
    return serialisedTile.append(std::string(1, letter)).append("-").append(
            std::to_string(value));
}

Tile::~Tile() = default;
