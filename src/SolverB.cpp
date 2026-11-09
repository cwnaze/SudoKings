#include "SolverB.h"

#include <chrono>
#include <utility>

namespace sudoku {
    SolverB::SolverB(const int stepLimit) : stepLimit_(stepLimit) {}

    std::string SolverB::name() const {
        return "Solver B (fewest-options-first)";
    }

    SolveResult SolverB::solve(Board& board) {
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

    std::optional<CellChoice> SolverB::selectCell(const Board& board) {
        std::optional<CellChoice> best;

        for (int r = 0; r < boardSize; ++r) {
            for (int c = 0; c < boardSize; ++c) {
                if (!board.isEmpty(r, c)) continue;

                std::vector<int> cands = board.candidates(r, c);
                if (!best || cands.size() < best->candidates.size()) {
                    best = CellChoice{Position{r, c}, std::move(cands)};
                    if (best->candidates.empty()) return best;
                }
            }
        }

        return best;
    }

    bool SolverB::search(Board& board) {
        const auto choice = selectCell(board);
        if (!choice) return true;                   
        if (choice->candidates.empty()) return false; 

        const int row = choice->pos.row;
        const int col = choice->pos.col;
        const bool isGuess = choice->candidates.size() >= 2;

        for (const int value : choice->candidates) {
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

    std::string SolverB::toGridString(const Board& board) {
        std::string grid;
        grid.reserve(cellCount);
        for (int r = 0; r < boardSize; ++r) {
            for (int c = 0; c < boardSize; ++c) {
                const int v = board.get(r, c);
                grid += (v == 0) ? '.' : static_cast<char>('0' + v);
            }
        }
        return grid;
    }
}