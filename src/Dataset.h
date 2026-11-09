#pragma once
 
#include <string>
#include <vector>
 
using namespace std;
 
// one row of the Kaggle file. puzzle: start, its solution answer, and clue count.

struct puz {
    string start;
    string answer;
    int clues;
};
 
// counts the given clues in an 81 character puzzle.

int clueCounts(string start);
 
// reads up to n puzzles from the kaggle CSV, starting at the top of the file.
// a smaller n gives a smaller sample. each line will be "puzzle,solution".
// a valid grid is exactly 81 digits, 0 means a blank cell, lines that are not
// two valid grids are skipped
// will return an empty vector if the file cannot be opened, so the caller should check size().

vector<puz> loadData(string file, int n);