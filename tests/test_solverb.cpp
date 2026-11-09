
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

#include "Board.h"
#include "SolverB.h"

using sudoku::Board;
using sudoku::CellChoice;
using sudoku::SolverB;

namespace {

    const std::string kSolution =
        "534678912672195348198342567859761423426853791713924856961537284287419635345286179";
    const std::string kEasyPuzzle =
        "530070000600195000098000060800060003400803001700020006060000280000419005000080079";
    const std::string kHardPuzzle =
        "800000000003600000070090200050007000000045700000100030001000068008500010090000400";

    Board boardWith(const std::vector<std::tuple<int, int, int>>& cells) {
        std::string s(81, '0');
        for (const auto& [r, c, v] : cells) {
            s[r * 9 + c] = static_cast<char>('0' + v);
        }
        return Board(s);
    }

    Board solutionMinus(const std::vector<std::pair<int, int>>& blanks) {
        std::string s = kSolution;
        for (const auto& [r, c] : blanks) s[r * 9 + c] = '0';
        return Board(s);
    }

    bool contains(const std::vector<int>& v, int x) {
        return std::find(v.begin(), v.end(), x) != v.end();
    }

    void requireValidMrvChoice(const Board& board) {
        const auto choice = SolverB::selectCell(board);
        REQUIRE(choice.has_value());

        const int row = choice->pos.row;
        const int col = choice->pos.col;
        REQUIRE(board.isEmpty(row, col));
        REQUIRE(choice->candidates == board.candidates(row, col));

        const std::size_t chosenCount = choice->candidates.size();
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (!board.isEmpty(r, c)) continue;
                const std::size_t n = board.candidates(r, c).size();
                REQUIRE(n >= chosenCount);
                if (r * 9 + c < row * 9 + col) REQUIRE(n > chosenCount);
            }
        }
    }

}


TEST_CASE("candidates: empty board allows every digit in ascending order", "[solverb][candidates]") {
    const Board board;
    const std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    REQUIRE(board.candidates(0, 0) == expected);
    REQUIRE(board.candidates(4, 4) == expected);
    REQUIRE(board.candidates(8, 8) == expected);
}

TEST_CASE("candidates: a digit is pruned from its row", "[solverb][candidates]") {
    const Board board = boardWith({{0, 0, 5}});
    REQUIRE_FALSE(contains(board.candidates(0, 8), 5));
    REQUIRE(board.candidates(0, 8).size() == 8);
}

TEST_CASE("candidates: a digit is pruned from its column", "[solverb][candidates]") {
    const Board board = boardWith({{0, 0, 5}});
    REQUIRE_FALSE(contains(board.candidates(8, 0), 5));
    REQUIRE(board.candidates(8, 0).size() == 8);
}

TEST_CASE("candidates: a digit is pruned from its 3x3 box", "[solverb][candidates]") {
    const Board board = boardWith({{0, 0, 5}});
    REQUIRE_FALSE(contains(board.candidates(2, 2), 5));
    REQUIRE(board.candidates(2, 2).size() == 8);
}

TEST_CASE("candidates: cells that are not peers keep the digit", "[solverb][candidates]") {
    const Board board = boardWith({{0, 0, 5}});
    REQUIRE(contains(board.candidates(4, 4), 5));
    REQUIRE(board.candidates(4, 4).size() == 9);
    REQUIRE(contains(board.candidates(3, 3), 5));
    REQUIRE(contains(board.candidates(8, 8), 5));
}

TEST_CASE("candidates: row, column and box eliminations combine", "[solverb][candidates]") {
    const Board board = boardWith({{4, 0, 1}, {0, 4, 2}, {3, 3, 3}});
    const std::vector<int> expected = {4, 5, 6, 7, 8, 9};
    REQUIRE(board.candidates(4, 4) == expected);
}

TEST_CASE("candidates: a filled cell has no candidates", "[solverb][candidates]") {
    const Board board = boardWith({{4, 4, 7}});
    REQUIRE(board.candidates(4, 4).empty());
}

TEST_CASE("candidates: a cell whose row and column cover all digits is a dead end", "[solverb][candidates]") {
    const Board board = boardWith({{8, 0, 1}, {8, 1, 2}, {8, 2, 3}, {8, 3, 4},
                                   {8, 4, 5}, {8, 5, 6}, {8, 6, 7}, {8, 7, 8}, {0, 8, 9}});
    REQUIRE(board.candidates(8, 8).empty());
}

TEST_CASE("candidates: placing shrinks the list and clearing restores it", "[solverb][candidates]") {
    Board board;
    const std::vector<int> before = board.candidates(0, 1);
    REQUIRE(before.size() == 9);

    REQUIRE(board.place(0, 0, 4));
    REQUIRE_FALSE(contains(board.candidates(0, 1), 4));
    REQUIRE(board.candidates(0, 1).size() == 8);

    REQUIRE(board.clear(0, 0));
    REQUIRE(board.candidates(0, 1) == before);
}

TEST_CASE("candidates: a solved grid with one blank leaves exactly the right digit", "[solverb][candidates]") {
    const Board board = solutionMinus({{6, 7}});
    const std::vector<int> expected = {kSolution[6 * 9 + 7] - '0'};
    REQUIRE(board.candidates(6, 7) == expected);
}


TEST_CASE("selectCell: a full board has nothing to choose", "[solverb][mrv]") {
    const Board board(kSolution);
    REQUIRE_FALSE(SolverB::selectCell(board).has_value());
}

TEST_CASE("selectCell: on an empty board every cell ties, so the first cell wins", "[solverb][mrv]") {
    const auto choice = SolverB::selectCell(Board());
    REQUIRE(choice.has_value());
    REQUIRE(choice->pos.row == 0);
    REQUIRE(choice->pos.col == 0);
    REQUIRE(choice->candidates.size() == 9);
}

TEST_CASE("selectCell: picks the most constrained cell, not the first empty one", "[solverb][mrv]") {
    const Board board = boardWith({{8, 0, 1}, {8, 1, 2}, {8, 2, 3}, {8, 3, 4},
                                   {8, 4, 5}, {8, 5, 6}, {8, 6, 7}, {8, 7, 8}});
    const auto choice = SolverB::selectCell(board);
    REQUIRE(choice.has_value());
    REQUIRE(choice->pos.row == 8);
    REQUIRE(choice->pos.col == 8);
    const std::vector<int> expected = {9};
    REQUIRE(choice->candidates == expected);
}

TEST_CASE("selectCell: returns a dead-end cell (0 candidates) even when it is last", "[solverb][mrv]") {
    const Board board = boardWith({{8, 0, 1}, {8, 1, 2}, {8, 2, 3}, {8, 3, 4},
                                   {8, 4, 5}, {8, 5, 6}, {8, 6, 7}, {8, 7, 8}, {0, 8, 9}});
    const auto choice = SolverB::selectCell(board);
    REQUIRE(choice.has_value());
    REQUIRE(choice->pos.row == 8);
    REQUIRE(choice->pos.col == 8);
    REQUIRE(choice->candidates.empty());
}

TEST_CASE("selectCell: ties go to the first cell in row-major order", "[solverb][mrv]") {
    const Board board = solutionMinus({{6, 7}, {2, 3}});
    const auto choice = SolverB::selectCell(board);
    REQUIRE(choice.has_value());
    REQUIRE(choice->pos.row == 2);
    REQUIRE(choice->pos.col == 3);
    const std::vector<int> expected = {kSolution[2 * 9 + 3] - '0'};
    REQUIRE(choice->candidates == expected);
}

TEST_CASE("selectCell: a single blank is chosen with its one legal digit", "[solverb][mrv]") {
    const Board board = solutionMinus({{6, 7}});
    const auto choice = SolverB::selectCell(board);
    REQUIRE(choice.has_value());
    REQUIRE(choice->pos.row == 6);
    REQUIRE(choice->pos.col == 7);
    const std::vector<int> expected = {kSolution[6 * 9 + 7] - '0'};
    REQUIRE(choice->candidates == expected);
}

TEST_CASE("selectCell: candidates match Board and every one is placeable", "[solverb][mrv]") {
    const Board board(kEasyPuzzle);
    const auto choice = SolverB::selectCell(board);
    REQUIRE(choice.has_value());
    REQUIRE(choice->candidates == board.candidates(choice->pos.row, choice->pos.col));
    for (const int v : choice->candidates) {
        REQUIRE(board.canPlace(choice->pos.row, choice->pos.col, v));
    }
}

TEST_CASE("selectCell: does not modify the board", "[solverb][mrv]") {
    const Board board(kHardPuzzle);
    const Board copy = board;
    SolverB::selectCell(board);
    REQUIRE(board == copy);
}

TEST_CASE("selectCell: follows the MRV rules at every step of a walk", "[solverb][mrv]") {
    for (const std::string& puzzle : {kEasyPuzzle, kHardPuzzle}) {
        Board board(puzzle);
        while (true) {
            const auto choice = SolverB::selectCell(board);
            if (!choice || choice->candidates.empty()) break;
            requireValidMrvChoice(board);
            REQUIRE(board.place(choice->pos.row, choice->pos.col, choice->candidates.front()));
        }
    }
}