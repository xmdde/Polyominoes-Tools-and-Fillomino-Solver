#ifndef POLYOMINOES_FILLOMINO_H
#define POLYOMINOES_FILLOMINO_H

#include <cstdint>
#include <string>
#include <set>
#include <vector>

#include "cell.h"
#include "utils.h"

class Fillomino {
    int rows;
    int cols;
    static constexpr bool opt = true;
    // vec of partially finished polyominoes??

    void getBoardFromFile(const std::string& file);
    uint8_t getPartialSize(const uint8_t i, const uint8_t j, std::vector<std::pair<uint8_t, uint8_t>>& area) const;
    uint8_t getPartialSize(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const;
    void getPartialSizeHelp(const std::vector<std::vector<Cell>>& b, uint8_t& size, std::vector<std::vector<bool>>& used, const uint8_t i,
                            const uint8_t j) const;
    bool isInBounds(const uint8_t i, const uint8_t j) const;
    bool isValid(const std::vector<std::vector<Cell>>& b) const;
    void getAllNbhs(const uint8_t i1, const uint8_t j1, const uint8_t i2, const uint8_t j2, std::vector<std::pair<uint8_t, uint8_t>>& nbhs) const;
    bool isCellBlocked(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const;
    bool invalidEmptyCells(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const;  // 2 cells
    void getSurroundingCells(const std::vector<std::pair<int, int>>& polyomino, std::set<std::pair<int, int>>& surr) const;

public:
    std::vector<std::vector<Cell>> board;
    Fillomino(const std::string& file) {
        getBoardFromFile(file);
    }
    Fillomino(const Fillomino& other) : rows(other.rows), cols(other.cols), board(other.board) {}
    Fillomino(const int r, const int c, const std::vector<std::vector<Cell>>& b) : rows(r), cols(c), board(b) {}

    void print() const;
    bool isValid() const;
    bool isSolved() const;
    bool processCode(const std::string& code, const uint8_t i, const uint8_t j, std::vector<std::vector<Cell>>& b) const;
    bool isCellAClue(const uint8_t i, const uint8_t j) const;
    uint8_t getNum(const uint8_t i, const uint8_t j) const;

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }
};

#endif  // POLYOMINOES_FILLOMINO_H
