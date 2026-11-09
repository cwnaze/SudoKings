#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace sudoku {
    struct puz {
        std::string start;
        std::string answer;
        int clues;
    };

    class Dataset {
    public:
        Dataset();
        bool load(const std::string& file, int n);
        void clear();
        std::size_t size() const;
        bool empty() const;
        const puz& at(std::size_t i) const;
        const puz& operator[](std::size_t i) const;
        std::vector<puz>::const_iterator begin() const;
        std::vector<puz>::const_iterator end() const;
        static int clueCounts(const std::string& start);
    private:
        static constexpr std::size_t gridLength = 81;
        static bool isGrid(const std::string& s, char lowest);
        static bool matchesAnswer(const std::string& start, const std::string& answer);
        static void trim(std::string& s);
        std::vector<puz> puzzles;
    };
}