//
// Created by Casey Nazelrod on 11/8/26.
//

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "Board.h"

using sudoku::Board;

namespace {

    const std::string kSolution =
        "534678912672195348198342567859761423426853791713924856961537284287419635345286179";
    const std::string kEasyPuzzle =
        "530070000600195000098000060800060003400803001700020006060000280000419005000080079";

    Board boardWith(const std::vector<std::tuple<int, int, int>>& cells) {
        std::string s(81, '0');
        for (const auto& [r, c, v] : cells) {
            s[r * 9 + c] = static_cast<char>('0' + v);
        }
        return Board(s);
    }

}


TEST_CASE("parse: cells read back and nonzero cells are givens", "[board][parse]") {
    const Board board(kEasyPuzzle);
    for (int i = 0; i < 81; ++i) {
        const int r = i / 9;
        const int c = i % 9;
        REQUIRE(board.get(r, c) == kEasyPuzzle[i] - '0');
        REQUIRE(board.isGiven(r, c) == (kEasyPuzzle[i] != '0'));
    }
}

TEST_CASE("parse: '.' and '0' both mean empty", "[board][parse]") {
    std::string dotted = kEasyPuzzle;
    for (char& ch : dotted) if (ch == '0') ch = '.';
    REQUIRE(Board(dotted) == Board(kEasyPuzzle));
}

TEST_CASE("parse: wrong length is rejected", "[board][parse]") {
    REQUIRE_THROWS_AS(Board(""), std::invalid_argument);
    REQUIRE_THROWS_AS(Board(std::string(80, '0')), std::invalid_argument);
    REQUIRE_THROWS_AS(Board(std::string(82, '0')), std::invalid_argument);
}

TEST_CASE("parse: invalid characters are rejected", "[board][parse]") {
    for (const char bad : {'/', ':', 'a', ' '}) {
        std::string s = kEasyPuzzle;
        s[2] = bad;
        REQUIRE_THROWS_AS(Board(s), std::invalid_argument);
    }
}

TEST_CASE("parse: conflicting givens are rejected", "[board][parse]") {
    REQUIRE_THROWS_AS(boardWith({{0, 0, 5}, {0, 8, 5}}), std::invalid_argument);  // row
    REQUIRE_THROWS_AS(boardWith({{0, 4, 7}, {8, 4, 7}}), std::invalid_argument);  // column
    REQUIRE_THROWS_AS(boardWith({{3, 3, 2}, {5, 5, 2}}), std::invalid_argument);  // box
}


TEST_CASE("canPlace: bad values and out-of-bounds cells are never legal", "[board][legality]") {
    const Board board;
    REQUIRE_FALSE(board.canPlace(4, 4, 0));
    REQUIRE_FALSE(board.canPlace(4, 4, 10));
    REQUIRE_FALSE(board.canPlace(-1, 0, 1));
    REQUIRE_FALSE(board.canPlace(0, 9, 1));
    REQUIRE(board.canPlace(4, 4, 1));
    REQUIRE(board.canPlace(4, 4, 9));
}

TEST_CASE("canPlace: row, column and box conflicts block a digit", "[board][legality]") {
    const Board board = boardWith({{2, 2, 5}});
    REQUIRE_FALSE(board.canPlace(2, 8, 5));  // same row
    REQUIRE_FALSE(board.canPlace(8, 2, 5));  // same column
    REQUIRE_FALSE(board.canPlace(0, 0, 5));  // same box
    REQUIRE(board.canPlace(3, 3, 5));        // next box over
    REQUIRE_FALSE(board.canPlace(2, 2, 5));  // filled cell
}

TEST_CASE("place: illegal moves are refused and change nothing", "[board][legality]") {
    Board board(kEasyPuzzle);
    REQUIRE_FALSE(board.place(0, 2, 5));   // conflict
    REQUIRE_FALSE(board.place(0, 0, 1));   // given
    REQUIRE_FALSE(board.place(9, 9, 1));   // out of bounds
    REQUIRE(board == Board(kEasyPuzzle));

    REQUIRE(board.place(0, 2, 4));
    REQUIRE_FALSE(board.place(0, 2, 1));   // already filled
    REQUIRE(board.get(0, 2) == 4);
    REQUIRE_FALSE(board.isGiven(0, 2));
}

TEST_CASE("clear: removes placed digits only", "[board][legality]") {
    Board board(kEasyPuzzle);
    REQUIRE_FALSE(board.clear(0, 0));      // given
    REQUIRE_FALSE(board.clear(0, 2));      // empty
    REQUIRE(board.place(0, 2, 4));
    REQUIRE(board.clear(0, 2));
    REQUIRE(board.isEmpty(0, 2));
}

TEST_CASE("reset: clears placements and keeps givens", "[board][legality]") {
    Board board(kEasyPuzzle);
    REQUIRE(board.place(0, 2, 4));
    board.reset();
    REQUIRE(board == Board(kEasyPuzzle));
}

TEST_CASE("get: out-of-bounds throws out_of_range", "[board][legality]") {
    const Board board;
    REQUIRE_THROWS_AS(board.get(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(board.get(0, 9), std::out_of_range);
}


TEST_CASE("findFirstEmpty: returns the first blank in row-major order", "[board][queries]") {
    const auto first = Board(kEasyPuzzle).findFirstEmpty();
    REQUIRE(first.has_value());
    REQUIRE(first->row == 0);
    REQUIRE(first->col == 2);

    const auto origin = Board().findFirstEmpty();
    REQUIRE(origin.has_value());
    REQUIRE(origin->row == 0);
    REQUIRE(origin->col == 0);

    REQUIRE_FALSE(Board(kSolution).findFirstEmpty().has_value());
}

TEST_CASE("emptyCount: tracks place and clear", "[board][queries]") {
    Board board(kEasyPuzzle);
    REQUIRE(board.emptyCount() == 51);
    REQUIRE(board.place(0, 2, 4));
    REQUIRE(board.emptyCount() == 50);
    REQUIRE(board.clear(0, 2));
    REQUIRE(board.emptyCount() == 51);
    REQUIRE(Board().emptyCount() == 81);
    REQUIRE(Board(kSolution).emptyCount() == 0);
}

TEST_CASE("candidates: exact list for an empty cell, none for filled or out of bounds", "[board][queries]") {
    const Board board(kEasyPuzzle);
    const std::vector<int> expected = {1, 2, 4};
    REQUIRE(board.candidates(0, 2) == expected);
    REQUIRE(board.candidates(0, 0).empty());
    REQUIRE(board.candidates(-1, 0).empty());
}

TEST_CASE("candidates: a dead-end cell has no candidates", "[board][queries]") {
    const Board board = boardWith({{0, 0, 1}, {0, 1, 2}, {0, 2, 3}, {0, 3, 4},
                                   {0, 4, 5}, {0, 5, 6}, {0, 6, 7}, {0, 7, 8}, {8, 8, 9}});
    REQUIRE(board.candidates(0, 8).empty());
}