#include "Dataset.h"

#include <fstream>
#include <regex>

int clueCounts(string start) {
    int count = 0;
    for (int i = 0; i < (int)start.length(); i++) {
        if (start[i] != '0') {
            count = count + 1;
        }
    }
    return count;
}

vector<puz> loadData(string file, int n) {
    vector<puz> puzzles;
    ifstream input(file);
    if (!input.is_open()) {
        return puzzles;
    }

    regex pattern("^[0-9]{81}$");

    string line;
    while ((int)puzzles.size() < n && getline(input, line)) {
        if (line.length() > 0 && line[line.length() - 1] == '\r') {
            line = line.substr(0, line.length() - 1);
        }

        int comma = line.find(',');
        if (comma != -1) {
            puz p;
            p.start = line.substr(0, comma);
            p.answer = line.substr(comma + 1);

            if (regex_match(p.start, pattern) && regex_match(p.answer, pattern)) {
                p.clues = clueCounts(p.start);
                puzzles.push_back(p);
            }
        }
    }
    return puzzles;
}