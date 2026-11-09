#include "SolverB.h"

#include <chrono>
#include <utility>
#include <vector>

using sudoku::Board;
using sudoku::Position;

namespace {
std::string boardToGrid(const Board& board) {
    std::string grid;
    grid.reserve(sudoku::cellCount);
    for (int r = 0; r < sudoku::boardSize; ++r) {
        for (int c = 0; c < sudoku::boardSize; ++c) {
            grid += static_cast<char>('0' + board.get(r, c));
        }
    }
    return grid;
}
}

std::string SolverB::name() const {
    return "Fewest-Options-First (MRV)";
}

SolveResult SolverB::solve(Board& board) {
    steps_ = 0;
    guesses_ = 0;
    backtracks_ = 0;
    hitLimit_ = false;

    const auto start = std::chrono::steady_clock::now();
    const bool solved = search(board);
    const auto end = std::chrono::steady_clock::now();

    SolveResult result;
    result.solved = solved;
    result.hitStepLimit = hitLimit_;
    result.grid = boardToGrid(board);
    result.steps = steps_;
    result.guesses = guesses_;
    result.backtracks = backtracks_;
    result.micros = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return result;
}

bool SolverB::search(Board& board) {
    bool found = false;
    Position best{0, 0};
    std::vector<int> bestCands;

    for (int r = 0; r < sudoku::boardSize; ++r) {
        for (int c = 0; c < sudoku::boardSize; ++c) {
            if (!board.isEmpty(r, c)) continue;

            std::vector<int> cands = board.candidates(r, c);
            if (!found || cands.size() < bestCands.size()) {
                found = true;
                best = Position{r, c};
                bestCands = std::move(cands);
            }
            if (bestCands.size() <= 1) break;
        }
        if (found && bestCands.size() <= 1) break;
    }

    if (!found) return true;          
    if (bestCands.empty()) return false; 

    const bool isGuess = bestCands.size() >= 2;

    for (int value : bestCands) {
        if (steps_ >= STEP_LIMIT) {
            hitLimit_ = true;
            return false;
        }

        board.place(best.row, best.col, value); 
        ++steps_;
        if (isGuess) ++guesses_;

        if (search(board)) return true;

        board.clear(best.row, best.col);
        if (hitLimit_) return false; 
        ++backtracks_;
    }

    return false;
}