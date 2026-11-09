cmake_minimum_required(VERSION 4.3)
project(SudoKings)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

#Catch2
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.16.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests src/Board.h
        src/Board.cpp
        src/Solver.h
        src/SolverA.h
        src/SolverA.cpp
        src/SolverB.h
        src/SolverB.cpp
        src/SolveResult.h
        src/Dataset.h
        src/Dataset.cpp
        tests/test_solvera.cpp
        tests/test_solvera.cpp
        tests/test_board.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)

add_executable(SudoKings src/main.cpp
        src/Board.h
        src/Solver.h
        src/SolverA.h
        src/SolverA.cpp
        src/SolverB.h
        src/SolverB.cpp
        src/SolveResult.h
        src/Board.cpp
        src/Dataset.h
        src/Dataset.cpp)