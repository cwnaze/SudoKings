cmake_minimum_required(VERSION 4.3)
project(SudoKings)

set(CMAKE_CXX_STANDARD 14)

#Catch2
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.16.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)

include_directories(src)
add_executable(SudoKings src/main.cpp)