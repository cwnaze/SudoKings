#pragma once

#include <string>
#include "Board.h"
#include "SolveResult.h"   // will rename if needed 
class Solver {
public:
    virtual ~Solver() = default;

    virtual SolveResult solve(Board& board) = 0;

    virtual std::string name() const = 0;
};