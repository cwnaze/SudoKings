//
// Created by Casey Nazelrod on 11/8/26.
// Conventions:
// Rows and columns are 0-8
// Cell values are 1-9. 0 means the cell is empty
// Boxes are numbered 0-8 (left to right, top to bottom) and box = (row/3)*3 + (col/3)
//

#pragma once
#include <string>
#include <array>
#include <ostream>
#include <optional>

namespace sudoku {
    constexpr int boardSize = 9;
    constexpr int boxSize = 3;
    constexpr int cellCount = boardSize * boardSize;

    struct Position {
        int row;
        int col;
    };

    class Board {
    public:
        Board(); //empty board
        explicit Board(const std::string& puzzle); //board from 81 char puzzle string
        //reading cells
        int get(int row, int col) const;
        bool isEmpty(int row, int col) const;
        bool isGiven(int row, int col) const;
        //changing cells
        bool place(int row, int col, int value);
        bool clear(int row, int col);
        void reset();
        //searching
        std::optional<Position> findFirstEmpty() const;
        int emptyCount() const;
        //checks
        bool canPlace(int row, int col, int value) const;
        bool isComplete() const;
        bool isValid() const;
        bool isSolved() const;
        //output
        std::string toString() const;
        friend std::ostream& operator<<(std::ostream& os, const Board& board);
        bool operator==(const Board& board) const;
        //helpers
        static int boxIndex(int row, int col);
    private:
        std::array<int, cellCount> cells;
        std::array<bool, cellCount> given; //true for cells that were clues in original puzzle

    };
}
