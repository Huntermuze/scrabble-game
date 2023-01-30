#ifndef ASSIGNMENT_2_NEW_SERIALISABLE_H
#define ASSIGNMENT_2_NEW_SERIALISABLE_H

#include <iostream>

/*
 * An abstract class which delineates that a class IS serialisable, and that
 * you can be guaranteed that it will contain methods to serialise itself into
 * the requested form. For the scope of this assignment, it only contains a
 * String version.
 *
 * A java or C# type interface would've been the preferred
 * choice over an abstract class, since the Serialisable class and its children
 * aren't necessarily related (there is no "is a" relationship nor extension).
 * It is more of a contract that ensures the implementing classes do indeed
 * implement the serialiseToString() method, and thus, can be used
 * polymorphically, which is very nice.
 */
class Serialisable {
public:
    /*
     * Abstract serialise method whose serialised form is a c++ string.
     */
    virtual std::string serialiseToString() = 0;

    /*
     * Abstract destructor method to keep the compiler happy (definition found
     * in Serialisable.cpp).
     */
    virtual ~Serialisable() = 0;
};


#endif //ASSIGNMENT_2_NEW_SERIALISABLE_H
