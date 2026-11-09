#pragma once
 
#include <string>
#include "Solver.h"
 
class SolverB : public Solver {
public:
    SolveResult solve(sudoku::Board& board) override;
    std::string name() const override;
 
private:
    bool search(sudoku::Board& board);
 
    int steps_ = 0;
    int guesses_ = 0;
    int backtracks_ = 0;
    bool hitLimit_ = false;
};
 