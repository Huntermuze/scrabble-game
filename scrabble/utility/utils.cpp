#include "utils.h"

bool utils::doesFileExist(const std::string &fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void utils::fileExistsElseThrow(const std::string &fileName) {
    if (!doesFileExist(fileName)) {
        throw std::runtime_error(
                "The filename \"" + fileName + "\" does not exist!");
    }
}

std::vector<std::string> *utils::splitString(const std::string &stringToSplit,
                                             const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;

    auto *result = new std::vector<std::string>();

    while ((pos_end = stringToSplit.find(delimiter, pos_start)) !=
           std::string::npos) {
        token = stringToSplit.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        result->push_back(token);
    }

    result->push_back(stringToSplit.substr(pos_start));
    return result;
}

std::string utils::toLower(const std::string &toConvert) {
    std::locale loc;
    std::string newString;

    for (char i: toConvert) {
        newString += std::tolower(i, loc);
    }

    return newString;
}

// Gets leftmost index of a word very quickly (non-recursive binary search).
int utils::getIndexOfWord(std::vector<std::string> *listOfWords, const std::string &word) {
    int low = 0;
    int listSize = listOfWords->size();
    int high = listSize;

    while (low < high) {
        int midpoint = (low + high) / 2;

        if ((*listOfWords)[midpoint] < word) {
            low = midpoint + 1;
        } else {
            high = midpoint;
        }
    }

    return low;
}

int utils::maximum(int n, int m) {
    return n > m ? n : m;
}

int utils::minimum(int n, int m) {
    return n < m ? n : m;
}



