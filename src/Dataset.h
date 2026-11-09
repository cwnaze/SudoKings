#pragma once

#include <fstream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

// one row of the Kaggle file. puzzle: start and its solution answer.

struct puz {
    string start;
    string answer;
};

// reads every puzzle from the kaggle CSV, each line will be "puzzle,solution".
// a valid grid is exactly 81 digits, 0 means a blank cell, lines that are not
// two valid grids are skipped
// will return an empty vector if the file cannot be opened, so the caller should check size().

vector<puz> loadData(string file) {
    vector<puz> puzzles;
    ifstream input(file);
    if (!input.is_open()) {
        return puzzles;
    }

    regex pattern("^[0-9]{81}$");

    string line;
    while (getline(input, line)) {
        if (line.length() > 0 && line[line.length() - 1] == '\r') {
            line = line.substr(0, line.length() - 1);
        }

        int comma = line.find(',');
        if (comma != -1) {
            puz p;
            p.start = line.substr(0, comma);
            p.answer = line.substr(comma + 1);

            if (regex_match(p.start, pattern) && regex_match(p.answer, pattern)) {
                puzzles.push_back(p);
            }
        }
    }
    return puzzles;
}