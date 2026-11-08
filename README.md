# SudoKings — Sudoku Solver Showdown

SudoKings solves Sudoku puzzles with two backtracking solvers and compares them:

- **Solver A:** plain backtracking. It tries each digit in an empty cell, recurses, and undoes the move when it hits a dead end.
- **Solver B:** TODO (name and one-line description of its optimization)

For each puzzle, the program prints the solved board and how each solver performed (TODO: time, nodes explored, etc.).

## Requirements

- C++14 compiler (GCC, Clang, or MSVC)
- CMake 3.16+
- Git (CMake downloads Catch2 automatically the first time it configures)

## Building

**CLion:** open the project folder, wait for CMake to load, select **SudoKings** in the run configuration dropdown, and click Build.

**Command line:**

```bash
cmake -S . -B build
cmake --build build
```

## Running

```bash
./build/SudoKings TODO-args
```

TODO: Describe the input (puzzle file path? stdin?) and its format, e.g. 9 lines of 9 digits with `0` for empty cells.

Example:

```
TODO: sample input
```

```
TODO: sample output
```

## Running the tests

**CLion:** select the **tests** run configuration and click Run.

**Command line:**

```bash
cmake --build build --target tests
./build/tests
```