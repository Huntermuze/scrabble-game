cmake_minimum_required(VERSION 3.21)
project(s3902219_A2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Assignment_2_NEW scrabble/menu.cpp
        scrabble/core/gameboard/Board.cpp
        scrabble/core/gameboard/Board.h
        scrabble/utility/LinkedList.h
        scrabble/utility/Node.h
        scrabble/core/Player.cpp
        scrabble/core/Player.h
        scrabble/core/Tile.h
        scrabble/core/Tile.cpp
        scrabble/utility/utils.cpp
        scrabble/utility/utils.h
        scrabble/core/Scrabble.cpp
        scrabble/core/Scrabble.h
        scrabble/utility/SimpleQueue.h
        scrabble/contract/Serialisable.h
        scrabble/contract/Serialisable.cpp
        scrabble/utility/typedefs.h
        scrabble/core/gameboard/BoardGridTraverser.cpp
        scrabble/core/gameboard/BoardGridTraverser.h)
