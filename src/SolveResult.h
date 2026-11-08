const int STEP_LIMIT = 1000000; // Maximum number of steps allowed for solving the puzzle, will be changed if need be. 

struct SolveResult {
    bool solved;
    bool hitStepLimit;
    string grid;
    int steps;
    int guesses;
    int backtracks;
    long long micros; 
};