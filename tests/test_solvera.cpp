//
// Created by Casey Nazelrod on 11/8/26.
//

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "Board.h"
#include "SolverA.h"

using sudoku::Board;
using sudoku::SolverA;

namespace {

    const std::string kSolution =
        "534678912672195348198342567859761423426853791713924856961537284287419635345286179";
    const std::string kEasyPuzzle =
        "530070000600195000098000060800060003400803001700020006060000280000419005000080079";
    const std::string kHardPuzzle =
        "800000000003600000070090200050007000000045700000100030001000068008500010090000400";
    const std::string kHardSolution =
        "812753649943682175675491283154237896369845721287169534521974368438526917796318452";
    const std::string kDeepContradiction =
        "002460000070200009009810560800090010193000008060008005200080750041500000000001940";

    Board boardWith(const std::vector<std::tuple<int, int, int>>& cells) {
        std::string s(81, '0');
        for (const auto& [r, c, v] : cells) {
            s[r * 9 + c] = static_cast<char>('0' + v);
        }
        return Board(s);
    }

}


TEST_CASE("solve: easy puzzle matches the expected solution", "[solvera][solve]") {
    Board board(kEasyPuzzle);
    SolverA solver;
    const SolveResult result = solver.solve(board);
    REQUIRE(result.solved);
    REQUIRE(result.grid == kSolution);
}

TEST_CASE("solve: hard puzzle matches the expected solution", "[solvera][solve]") {
    Board board(kHardPuzzle);
    SolverA solver;
    const SolveResult result = solver.solve(board);
    REQUIRE(result.solved);
    REQUIRE(result.grid == kHardSolution);
}

TEST_CASE("solve: an already solved board is returned unchanged", "[solvera][solve]") {
    Board board(kSolution);
    SolverA solver;
    const SolveResult result = solver.solve(board);
    REQUIRE(result.solved);
    REQUIRE(result.grid == kSolution);
}


TEST_CASE("reject: duplicate digit in a row", "[solvera][reject]") {
    REQUIRE_THROWS_AS(boardWith({{0, 0, 5}, {0, 8, 5}}), std::invalid_argument);
}

TEST_CASE("reject: duplicate digit in a column", "[solvera][reject]") {
    REQUIRE_THROWS_AS(boardWith({{0, 4, 7}, {8, 4, 7}}), std::invalid_argument);
}

TEST_CASE("reject: duplicate digit in a 3x3 box", "[solvera][reject]") {
    REQUIRE_THROWS_AS(boardWith({{3, 3, 2}, {5, 5, 2}}), std::invalid_argument);
}

TEST_CASE("reject: a cell with no legal digit", "[solvera][reject]") {
    Board board = boardWith({{8, 0, 1}, {8, 1, 2}, {8, 2, 3}, {8, 3, 4},
                             {8, 4, 5}, {8, 5, 6}, {8, 6, 7}, {8, 7, 8}, {0, 8, 9}});
    SolverA solver;
    REQUIRE_FALSE(solver.solve(board).solved);
}

TEST_CASE("reject: two cells forced to the same digit", "[solvera][reject]") {
    Board board = boardWith({{0, 0, 1}, {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5},
                             {0, 5, 6}, {0, 6, 7}, {3, 7, 8}, {6, 8, 8}});
    SolverA solver;
    REQUIRE_FALSE(solver.solve(board).solved);
}

TEST_CASE("reject: a contradiction found deep in the search", "[solvera][reject]") {
    Board board(kDeepContradiction);
    SolverA solver;
    REQUIRE_FALSE(solver.solve(board).solved);
}