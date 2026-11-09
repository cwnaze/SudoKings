//
// Created by Casey Nazelrod on 11/8/26.
//

#include "SolverA.h"

#include <chrono>
#include <vector>

namespace sudoku {
    SolverA::SolverA(const int stepLimit) : stepLimit_(stepLimit) {}

    std::string SolverA::name() const {
        return "Solver A (plain backtracking)";
    }

    SolveResult SolverA::solve(sudoku::Board& board) {
        steps_ = 0;
        guesses_ = 0;
        backtracks_ = 0;
        hitLimit_ = false;

        const auto start = std::chrono::steady_clock::now();

        bool solved = false;
        if (board.isValid()) solved = search(board);

        const auto end = std::chrono::steady_clock::now();

        if (!solved) board.reset();

        SolveResult result{};
        result.solved = solved;
        result.hitStepLimit = hitLimit_;
        result.grid = toGridString(board);
        result.steps = steps_;
        result.guesses = guesses_;
        result.backtracks = backtracks_;
        result.micros = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return result;
    }

    bool SolverA::search(sudoku::Board& board) {
        const auto pos = board.findFirstEmpty();
        if (!pos) return true;

        const int row = pos->row;
        const int col = pos->col;

        const std::vector<int> options = board.candidates(row, col);
        const bool isGuess = options.size() >= 2;

        for (const int value : options) {
            if (steps_ >= stepLimit_) {
                hitLimit_ = true;
                return false;
            }

            if (!board.place(row, col, value)) continue;

            ++steps_;
            if (isGuess) ++guesses_;

            if (search(board)) return true;

            if (hitLimit_) return false;

            board.clear(row, col);
            ++backtracks_;
        }

        return false;
    }

    std::string SolverA::toGridString(const sudoku::Board& board) {
        std::string grid;
        grid.reserve(sudoku::cellCount);
        for (int r = 0; r < sudoku::boardSize; ++r) {
            for (int c = 0; c < sudoku::boardSize; ++c) {
                const int v = board.get(r, c);
                grid += (v == 0) ? '.' : static_cast<char>('0' + v);
            }
        }
        return grid;
    }
}