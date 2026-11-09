#include <iostream>
#include <iomanip>
#include <string>
#include "Board.h"
#include "Dataset.h"

using namespace std;
using namespace sudoku;

const int WIDTH = 50;

// how many puzzles to load from the dataset when the program starts
const int LOAD_COUNT = 100000;

void borderPr() {
    cout << string(WIDTH, '=') << endl;
}

void dividerPr() {
    cout << string(WIDTH, '-') << endl;
}

void blPr() {
    cout << endl;
}

// prints the title in the middle of the line
void titlePr(string title) {
    int left = (WIDTH - (int)title.length()) / 2;
    cout << string(left, ' ') << title << endl;
}

void optionPr(int number, string text) {
    cout << "  " << number << ". " << text << endl;
}

// the big SUDOKU letters at the top of the menu (i thought it looked cool, didnt have time for colors.)
void bannerPr() {
    cout << "      ####  #   #  ####    ###   #   #  #   #" << endl;
    cout << "     #      #   #  #   #  #   #  #  #   #   #" << endl;
    cout << "      ###   #   #  #   #  #   #  ###    #   #" << endl;
    cout << "         #  #   #  #   #  #   #  #  #   #   #" << endl;
    cout << "     ####    ###   ####    ###   #   #   ### " << endl;
}

void menuPr() {
    cout << endl;
    borderPr();
    blPr();
    bannerPr();
    blPr();
    titlePr("S O L V E R   S H O W D O W N");
    blPr();
    borderPr();
    blPr();
    optionPr(1, "Load a puzzle from the dataset");
    optionPr(2, "Type in my own puzzle");
    optionPr(3, "Solve with Solver A");
    optionPr(4, "Solve with Solver B");
    optionPr(5, "Run benchmark on N puzzles");
    optionPr(6, "Quit");
    blPr();
    dividerPr();
    cout << "Choose 1-6: ";
}

// reads a number, returns -1 if the user typed something that is not a number
int numRead() {
    int number;
    cin >> number;
    if (cin.fail()) {
        if (cin.eof()) {
            return 6;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        return -1;
    }
    return number;
}

void puzPrint(string puzzle) {
    Board board(puzzle);
    cout << endl;
    cout << board;
}

int main() {
    Dataset data;
    bool loaded = data.load("sudoku.csv", LOAD_COUNT);
    if (!loaded) {
        loaded = data.load("../sudoku.csv", LOAD_COUNT);
        }
    if (!loaded) {
        cout << "Could not find sudoku.csv. ";
        string path;
        getline(cin, path);
        if (path != "") {
            loaded = data.load(path, LOAD_COUNT);
        }
    }
    if (!loaded || data.empty()) {
        cout << "No puzzles loaded." << endl;
    } 
    else {
        cout << "Loaded " << data.size() << " puzzles." << endl;
        }

    string puzzle = "";
    string answer = "";
    bool running = true;

    while (running) {
        menuPr();
        int choice = numRead();

        if (choice == 1) {
            if (data.empty()) {
                cout << "No dataset loaded." << endl;
            } 
            else {
                cout << "Row number (1 to " << data.size() << "): ";
                int row = numRead();
                if (row < 1 || row > (int)data.size()) {
                    cout << "That row does not exist." << endl;
                } else {
                    puzzle = data[row - 1].start;
                    answer = data[row - 1].answer;
                    cout << endl << " Loaded row " << row << " (" << data[row - 1].clues << " clues)";
                    puzPrint(puzzle);
                }
            }
        } 
        else if (choice == 2) {
            cout << "Type 81 digits (0 or . = blank): ";
            string typed;
            cin >> typed;

            // the dataset uses 0 for blank, so turn any . into 0
            for (int i = 0; i < (int)typed.length(); i++) {
                if (typed[i] == '.') {
                    typed[i] = '0';
                }
            }

            // board throws an error if the puzzle is not valid
            // try: attempt to build the board.
            // catch: if that failed, show the error message instead of crashing.
            try {
                Board test(typed);
                puzzle = typed;
                answer = "";
                cout << endl << " Loaded your puzzle (" << Dataset::clueCounts(puzzle) << " clues)";
                puzPrint(puzzle);
                } 
            catch (exception& e) {
                cout << "That puzzle is not valid: " << e.what() << endl;
                }
        } 
        else if (choice == 3) {
            cout << "Solving is not added yet." << endl;
        } 
        else if (choice == 4) {
            cout << "Solving is not added yet." << endl;
        } 
        else if (choice == 5) {
            if (data.empty()) {
                cout << "No dataset loaded." << endl;
            } 
            else {
                cout << "Benchmark is not added yet (issue #7)." << endl;
            }
        } 
        else if (choice == 6) {
            running = false;
        } 
        else {
            cout << "Please type a number from 1 to 6." << endl;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}