#pragma once

#include <optional>
#include <string>
#include <vector>
#include "Solver.h"

namespace sudoku {
    struct CellChoice {
        Position pos;
        std::vector<int> candidates;
    };
    class SolverB : public Solver {
    public:
        explicit SolverB(int stepLimit = STEP_LIMIT);
        SolveResult solve(Board& board) override;
        std::string name() const override;
        static std::optional<CellChoice> selectCell(const Board& board);

    private:
        bool search(Board& board);
        static std::string toGridString(const Board& board);

        int stepLimit_;
        int steps_ = 0;
        int guesses_ = 0;
        int backtracks_ = 0;
        bool hitLimit_ = false;
    };
}