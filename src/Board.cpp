//
// Created by Casey Nazelrod on 11/8/26.
//

#include "Board.h"
#include <sstream>
#include <stdexcept>

namespace sudoku {
    bool Board::inBounds(const int row, const int col) {
        return row >= 0 && row < boardSize && col >= 0 && col < boardSize;
    }

    int Board::indexOf(const int row, const int col) {
        if (!inBounds(row, col)) {
            throw std::out_of_range("Board index out of range: [" + std::to_string(row) + ", " + std::to_string(col) + "]");
        }

        return row * boardSize + col;
    }

   //parse and validate 81 char string
    Board::Board() = default;

    Board::Board(const std::string& puzzle) {
        if (puzzle.size() != cellCount) {
            throw std::invalid_argument("Puzzle must be 81 characters, got " + std::to_string(puzzle.size()));
        }

        for (int i = 0; i < cellCount; ++i) {
            const char ch = puzzle[i];
            const int row = i / boardSize;
            const int col = i % boardSize;

            if (ch == '0' || ch == '.') continue;

            if  (ch < '1' || ch > '9') {
                throw std::invalid_argument("Invalid character" + std::to_string(ch) + "at [" + std::to_string(row) + ", " + std::to_string(col) + "]");
            }

            const int value = ch - '0';
            if  (!canPlace(row, col, value)) {
                throw std::invalid_argument(std::string(1,ch) + "at [" + std::to_string(row) + ", " + std::to_string(col) + "] conflicts with another given");
            }

            cells[i] = value;
            given[i] = true;
        }
    }

    //reading and changing cells
    int Board::get(const int row, const int col) const {
       return cells[indexOf(row, col)];
    }

    bool Board::isEmpty(const int row, const int col) const {
        return get(row, col) == 0;
    }

    bool Board::isGiven(const int row, const int col) const {
        return given[indexOf(row, col)];
    }

    bool Board::place(const int row, const int col, const int value) {
        if (!inBounds(row, col) || !canPlace(row, col, value)) return false;
        cells[indexOf(row, col)] = value;
        return true;
    }

    bool Board::clear(int row, int col) {
        if (!inBounds(row, col)) return false;
        const int i = indexOf(row, col);
        if (given[i] || cells[i] == 0) return false;
        cells[i] = 0;
        return true;
    }

    void Board::reset() {
        for (size_t i = 0; i < cellCount; ++i) {
            if (!given[i]) cells[i] = 0;
        }
    }

    //row, column, 3x3 box legality checking
    bool Board::canPlace(const int row, const int col, const int value) const {
        if (!inBounds(row, col) || value < 1 || value > boardSize) return false;
        if (cells[indexOf(row, col)] != 0) return false;

        for (int i = 0; i < boardSize; ++i) {
            if (cells[indexOf(row, i)] == value) return false;
            if (cells[indexOf(i, row)] == value) return false;
        }

        const int boxRow = (row / boxSize) * boxSize;
        const int boxCol = (col / boxSize) * boxSize;
        for (int r = boxRow; r < boxRow + boxSize; ++r) {
            for (int c = boxCol; c < boxCol + boxSize; ++c) {
                if (cells[indexOf(r, c)] == value) return false;
            }
        }

        return true;
    }

    bool Board::isValid() const {
        for (int i = 0; i < boardSize; ++i) {
            std::array<bool, boardSize + 1> seenRow{};
            std::array<bool, boardSize + 1> seenCol{};
            std::array<bool, boardSize + 1> seenBox{};

            int boxRow = (i / boxSize) * boxSize;
            int boxCol = (i % boxSize) * boxSize;

            for (int k = 0; k < boardSize; ++k) {
                int rowVal = cells[indexOf(boxRow, k)];
                int colVal = cells[indexOf(k, i)];
                int boxVal = cells[indexOf(boxRow + k / boxSize, boxCol + k % boxSize)];

                if (rowVal != 0) {
                    if (seenRow[rowVal]) return false;
                    seenRow[rowVal] = true;
                }
                if (colVal != 0) {
                    if (seenCol[colVal]) return false;
                    seenCol[colVal] = true;
                }
                if (boxVal != 0) {
                    if (seenBox[boxVal]) return false;
                    seenBox[boxVal] = true;
                }
            }
        }

        return true;
    }

    bool Board:: isComplete() const {
        return emptyCount() == 0;
    }

    bool Board::isSolved() const {
        return isComplete() && isValid();
    }

    //find next empty and candidate digits queries
    std::optional<Position> Board::findFirstEmpty() const {
        for (int i = 0; i < cellCount; ++i) {
            if (cells[i] == 0) return Position{i / boardSize, i % boardSize};
        }

        return std::nullopt;
    }

    std::vector<int> Board::candidates(int row, int col) const {
        std::vector<int> result;
        for (int i = 1; i <= boardSize; ++i) {
            if (canPlace(row, col, i)) result.push_back(i);
        }

        return result;
    }

    int Board::emptyCount() const {
        int count = 0;
        for (int i : cells) {
            if (i == 0) ++count;
        }

        return count;
    }

    //print and to string for menu
    std::string Board::toString() const {
        std::ostringstream out;
        for (int r = 0; r < boardSize; ++r) {
            if (r != 0 && r % boxSize == 0) {
                out << "------+-------+------\n";
            }
            for (int c = 0; c < boardSize; ++c) {
                if (c != 0 && c % boxSize == 0) {
                    out << "| ";
                }
                int value = cells[indexOf(r,c)];
                out << (value == 0 ? '.' : static_cast<char>('0' + value));
                if (c != boardSize - 1) {
                    out << ' ' ;
                }
            }

            out << '\n';
        }

        return out.str();
    }

    std::ostream& operator<<(std::ostream& os, const Board& board) {
        return os << board.toString();
    }

    bool Board::operator==(const Board& board) const {
        return cells == board.cells;
    }
}
