#ifndef ASSIGNMENT_2_NEW_UTILS_H
#define ASSIGNMENT_2_NEW_UTILS_H

#include <iostream>
#include <fstream>
#include <vector>

namespace utils {
    /*
     * Check if file exists and return whether it does or not via a boolean.
     * Less aggressive form of file checking.
     *
     * PARAMS:
     *  filePath - absolute file path to the desired file.
     *
     *  Returns a boolean representing if the file exists or not.
     */
    bool doesFileExist(const std::string &filePath);

    /*
     * Check if file exists and if it does not, throw a runtime error.
     *
     * PARAMS:
     *  filePath - absolute file path to the desired file.
     */
    void fileExistsElseThrow(const std::string &filePath);

    /*
     * Split a string by a common delimiter.
     *
     *  PARAMS:
     *   stringToSplit - the string to split.
     *   delimiter - the delimiter to split the string on.
     *
     *  Returns a string vector containing the split.
     */
    std::vector<std::string> *splitString(const std::string &stringToSplit,
                                          const std::string &delimiter);

    /*
     * Convert a string's casing to its lowercase counterpart.
     *
     * PARAMS:
     *  toConvert - string to convert to lowercase.
     *
     * Returns the lowercased string.
     */
    std::string toLower(const std::string &toConvert);

    /*
     * Given a list of strings (listOfWords) and a string to find (word), find the index of that
     * string's occurrence in that list. This function utilises a non-recursive binary search, which
     * was designed to be as efficient as possible. In mathematical terms, it will continually bisect
     * the list, searching for the left-most occurrence of the word (in-case there are duplicates -
     * there are no duplicates in the provided list, as I have lowercased them all and removed all
     * dupes for maximum efficiency).
     */
    int getIndexOfWord(std::vector<std::string> *listOfWords, const std::string &word);

    // Given two numbers, n and m, returns the maximum of those two numbers.
    int maximum(int n, int m);

    // Given two numbers, n and m, returns the minimum of those two numbers.
    int minimum(int n, int m);
}

#endif //ASSIGNMENT_2_NEW_UTILS_H
