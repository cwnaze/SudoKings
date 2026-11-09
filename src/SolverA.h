//
// Created by Casey Nazelrod on 11/8/26.
//

#pragma once
#include "Solver.h"
#include <string>

namespace sudoku {
    class SolverA : public Solver {
    public:
        explicit SolverA(int stepLimit = STEP_LIMIT);
        SolveResult solve(Board& board) override;
        std::string name() const override;
    private:
        int stepLimit_;
        int steps_ = 0;
        int guesses_ = 0;
        int backtracks_ = 0;
        bool hitLimit_ = false;

        bool search(Board& board);
        static std::string toGridString(const Board& board);
    };
}