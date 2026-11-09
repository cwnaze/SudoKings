#include "Benchmark.h"

#include <fstream>
#include <iostream>
#include <string>
#include "Board.h"
#include "SolverA.h"
#include "SolverB.h"

using namespace std;
using namespace sudoku;

// totals for one solver over all the puzzles
struct Stats {
    string name;
    long long micros;
    long long steps;
    long long guesses;
    long long backtracks;
    int unsolved;
};

// adds the result of one solve to a solver's totals
void resultAdd(Stats& stats, SolveResult result, string answer) {
    stats.micros = stats.micros + result.micros;
    stats.steps = stats.steps + result.steps;
    stats.guesses = stats.guesses + result.guesses;
    stats.backtracks = stats.backtracks + result.backtracks;

    // a puzzle counts as unsolved if the solver failed
    // or its grid is different from the solution in the dataset
    if (!result.solved || result.grid != answer) {
        stats.unsolved = stats.unsolved + 1;
    }
}

// average of a total over n puzzles
double average(long long total, int n) {
    return (double)total / n;
}

void statsPr(Stats stats, int n) {
    cout << endl;
    cout << " " << stats.name << endl;
    cout << "   Avg time:       " << average(stats.micros, n) << " us" << endl;
    cout << "   Avg steps:      " << average(stats.steps, n) << endl;
    cout << "   Avg guesses:    " << average(stats.guesses, n) << endl;
    cout << "   Avg backtracks: " << average(stats.backtracks, n) << endl;
    cout << "   Unsolved:       " << stats.unsolved << endl;
}

// writes one row for a solver to the csv file
void saveStat(ofstream& file, Stats stats, int n) {
    file << stats.name << "," << n << ",";
    file << average(stats.micros, n) << ",";
    file << average(stats.steps, n) << ",";
    file << average(stats.guesses, n) << ",";
    file << average(stats.backtracks, n) << ",";
    file << stats.unsolved << endl;
}

void bmRun(Dataset& data, int n) {
    if (n > (int)data.size()) {
        n = (int)data.size();
    }
    if (n < 1) {
        return;
    }

    SolverA solverA;
    SolverB solverB;
    Stats statsA = {"Solver A", 0, 0, 0, 0, 0};
    Stats statsB = {"Solver B", 0, 0, 0, 0, 0};

    cout << " Running both solvers on " << n << " puzzles..." << endl;
    for (int i = 0; i < n; i++) {
        // each solver gets its own fresh copy of the puzzle
        Board boardA(data[i].start);
        Board boardB(data[i].start);
        resultAdd(statsA, solverA.solve(boardA), data[i].answer);
        resultAdd(statsB, solverB.solve(boardB), data[i].answer);
    }

    cout << endl << " BENCHMARK RESULTS (" << n << " puzzles)" << endl;
    statsPr(statsA, n);
    statsPr(statsB, n);

    ofstream file("results.csv");
    file << "solver,puzzles,avg_micros,avg_steps,avg_guesses,avg_backtracks,unsolved" << endl;
    saveStat(file, statsA, n);
    saveStat(file, statsB, n);
    cout << endl << " Results also saved to results.csv" << endl;
}