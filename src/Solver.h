#pragma once

#include <string>
#include "Board.h"
#include "SolveResult.h"

namespace sudoku {
    class Solver {
    public:
        virtual ~Solver() = default;

        virtual SolveResult solve(sudoku::Board& board) = 0;

        virtual std::string name() const = 0;
    };
}