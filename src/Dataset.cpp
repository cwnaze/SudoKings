#include "Dataset.h"

#include <fstream>
#include <utility>

namespace sudoku {
    Dataset::Dataset() = default;

    bool Dataset::load(const std::string& file, int n) {
        puzzles.clear();
        if (n <= 0) return true;

        std::ifstream input(file);
        if (!input.is_open()) return false;

        const std::size_t limit = static_cast<std::size_t>(n);
        std::string line;
        while (puzzles.size() < limit && std::getline(input, line)) {
            const std::size_t comma = line.find(',');
            if (comma == std::string::npos) continue;

            const std::size_t next = line.find(',', comma + 1);
            const std::size_t answerLen = (next == std::string::npos) ? std::string::npos : next - comma - 1;

            puz p;
            p.start = line.substr(0, comma);
            p.answer = line.substr(comma + 1, answerLen);
            trim(p.start);
            trim(p.answer);

            if (!isGrid(p.start, '0') || !isGrid(p.answer, '1') || !matchesAnswer(p.start, p.answer)) continue;

            p.clues = clueCounts(p.start);
            puzzles.push_back(std::move(p));
        }

        return true;
    }

    void Dataset::clear() {
        puzzles.clear();
    }

    std::size_t Dataset::size() const {
        return puzzles.size();
    }

    bool Dataset::empty() const {
        return puzzles.empty();
    }

    const puz& Dataset::at(const std::size_t i) const {
        return puzzles.at(i);
    }

    const puz& Dataset::operator[](const std::size_t i) const {
        return puzzles[i];
    }

    std::vector<puz>::const_iterator Dataset::begin() const {
        return puzzles.begin();
    }

    std::vector<puz>::const_iterator Dataset::end() const {
        return puzzles.end();
    }

    int Dataset::clueCounts(const std::string& start) {
        int count = 0;
        for (const char ch : start) {
            if (ch != '0') ++count;
        }

        return count;
    }

    bool Dataset::isGrid(const std::string& s, const char lowest) {
        if (s.size() != gridLength) return false;

        for (const char ch : s) {
            if (ch < lowest || ch > '9') return false;
        }

        return true;
    }

    bool Dataset::matchesAnswer(const std::string& start, const std::string& answer) {
        for (std::size_t i = 0; i < gridLength; ++i) {
            if (start[i] != '0' && start[i] != answer[i]) return false;
        }

        return true;
    }

    void Dataset::trim(std::string& s) {
        constexpr auto ws = " \t\r\n";
        const std::size_t first = s.find_first_not_of(ws);
        if (first == std::string::npos) {
            s.clear();
            return;
        }
        const std::size_t last = s.find_last_not_of(ws);
        s = s.substr(first, last - first + 1);
    }
}