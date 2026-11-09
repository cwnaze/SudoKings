# SudoKings — Sudoku Solver Showdown

SudoKings solves Sudoku puzzles with two backtracking solvers and compares them:

- **Solver A:** plain backtracking. It tries each digit in an empty cell, recurses, and undoes the move when it hits a dead end.
- **Solver B:** fewest-options-first backtracking. It always fills the empty cell with the fewest legal digits next, so it makes far fewer guesses and backtracks.

The program is an interactive menu. You load a puzzle from the dataset or type one in, solve it with either solver, and see how that solver performed: time, steps, guesses, backtracks, and whether its answer matches the dataset's solution. A benchmark option runs both solvers on the first N puzzles and saves the averages to `results.csv`.

What the numbers mean:

- **Step:** placing a legal digit in an empty cell.
- **Guess:** a step in a cell that had 2 or more legal digits.
- **Backtrack:** removing a placed digit after it led to a dead end.

Each solver stops after 10,000,000 steps (`STEP_LIMIT` in `SolveResult.h`) and reports `STEP LIMIT HIT`.

## Requirements

- C++17 compiler (GCC, Clang, or MSVC)
- CMake 3.16+
- Git (CMake downloads Catch2 automatically the first time it configures)
- `sudoku.csv`, the puzzle dataset (see [Dataset](#dataset))

## Building

**CLion:** open the project folder, wait for CMake to load, select **SudoKings** in the run configuration dropdown, and click Build.

**Command line:**

```bash
cmake -S . -B build
cmake --build build
```

## Dataset

The program reads puzzles from `sudoku.csv`. Each line is a puzzle and its solution, separated by a comma:

```
004300209005009001070060043006002087190007400050083000600000105003508690042910300,864371259325849761971265843436192587198657432257483916689734125713528694542916378
```

- Both fields are 81 digits, read left to right, top to bottom.
- `0` is an empty cell in the puzzle.
- Lines that don't fit this format (such as a header row) are skipped.
- Up to the first 100,000 valid puzzles are loaded (`LOAD_COUNT` in `main.cpp`).

At startup the program looks for `sudoku.csv` in the folder it is running from, then in the parent folder. If it finds neither, it asks for a path.

## Running

From the project folder, with `sudoku.csv` in that folder:

```bash
./build/SudoKings
```

The program takes no command-line arguments. Everything is done through the menu:

```
  1. Load a puzzle from the dataset
  2. Type in my own puzzle
  3. Solve with Solver A
  4. Solve with Solver B
  5. Run benchmark on N puzzles
  6. Quit
```

- **1:** pick a dataset row by number (starting at 1).
- **2:** type 81 characters, using `0` or `.` for empty cells. Puzzles with invalid characters or clashing clues are rejected.
- **3 / 4:** solve the loaded puzzle. On the solve screen, press `S` to show the solved grid or `M` to go back to the menu.
- **5:** run both solvers on the first N dataset puzzles.

### Example: solving a puzzle (row 1, Solver A)

```
      SUDOKU SOLVER SHOWDOWN - SOLVE SCREEN
==================================================

. . 4 | 3 . . | 2 . 9
. . 5 | . . 9 | . . 1
. 7 . | . 6 . | . 4 3
------+-------+------
. . 6 | . . 2 | . 8 7
1 9 . | . . 7 | 4 . .
. 5 . | . 8 3 | . . .
------+-------+------
6 . . | . . . | 1 . 5
. . 3 | 5 . 8 | 6 9 .
. 4 2 | 9 1 . | 3 . .

 Solver:      Solver A (plain backtracking)
 Clues:       35
 Time:        0.082 ms
 Steps:       181
 Guesses:     75
 Backtracks:  135
 Status:      SOLVED

 [S] show solved grid   [M] back to menu:
```

`Status` can be `SOLVED`, `WRONG ANSWER` (it solved the puzzle, but the result doesn't match the dataset's solution), `NOT SOLVED`, or `STEP LIMIT HIT`.

### Example: benchmark

```
 BENCHMARK RESULTS (1 puzzles)

 Solver A
   Avg time:       75 us
   Avg steps:      181
   Avg guesses:    75
   Avg backtracks: 135
   Unsolved:       0

 Solver B
   Avg time:       367 us
   Avg steps:      46
   Avg guesses:    0
   Avg backtracks: 0
   Unsolved:       0

 Results also saved to results.csv
```

Times vary by machine. The benchmark writes the same numbers to `results.csv`:

```
solver,puzzles,avg_micros,avg_steps,avg_guesses,avg_backtracks,unsolved
Solver A,1,75,181,75,135,0
Solver B,1,367,46,0,0,0
```

### Where `results.csv` goes

`results.csv` is written to the folder the program is **running from**, and it is overwritten on every benchmark run.

- **CLion:** the build folder (`cmake-build-debug/`), next to the executable. To put it in the project folder instead, set **Run → Edit Configurations → Working directory** to `$ProjectFileDir$`.
- **Command line:** whatever folder you ran the command from. If you run `./build/SudoKings` from the project folder, the file goes in the project folder.

## Running the tests

**CLion:** select the **tests** run configuration and click Run.

**Command line:**

```bash
cmake --build build --target tests
./build/tests
```