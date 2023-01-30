#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <typeinfo>
#include "../contract/Serialisable.h"

// Define a Letter type
typedef char Letter;
// Define a Value type
typedef int Value;

class Tile : public Serialisable {
public:
    /*
     * We do not need a move constructor for this class, since the fields are all
     * primitive. A copy will be just as efficient.
     */
    Tile(const Tile &other);

    Tile(Letter letter, Value val);

    Tile();

    ~Tile() override;

    Letter getLetter() const;

    void setLetter(Letter newLetter);

    Value getValue() const;

    void setValue(Value newValue);

    // Serialise object to its string representation.
    std::string serialiseToString() override;

    // Overload equality operator to check if fields are equivalent.
    bool operator==(const Tile &other) const;

private:
    Letter letter{};
    Value value{};
};

#endif // ASSIGN2_TILE_H
