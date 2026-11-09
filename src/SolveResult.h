#pragma once

#include <string>

namespace sudoku {
    constexpr int STEP_LIMIT = 10000000; // Maximum number of steps allowed for solving the puzzle, will be changed if need be.

    // step - placing a digit in an empty cell. only counted if placement is legal
    // guess - placing a digit in an empty cell that had 2+ legal digits. only counted if the cell had a choice
    // backtrack: removing a placed digit from a cell. only counted when it's removed because of a dead end

    struct SolveResult {
        bool solved;
        bool hitStepLimit;
        std::string grid;
        int steps; //definition above
        int guesses; //definition above
        int backtracks; //definition above
        long long micros; //time in microseconds
    };
}