#pragma once
 
#include "Dataset.h"
 
// runs both solvers on the first n puzzles in data, prints the results
// and saves them to results.csv
void bmRun(sudoku::Dataset& data, int n);