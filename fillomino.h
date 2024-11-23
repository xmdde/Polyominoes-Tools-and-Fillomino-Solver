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
    static constexpr bool opt = false;

    void getBoardFromFile(const std::string& file);
    int getPartialSize(const uint8_t i, const uint8_t j, std::vector<std::pair<uint8_t, uint8_t>>& area) const;
    void getPartialSizeHelp(const std::vector<std::vector<Cell>>& b, int& size, std::vector<std::vector<bool>>& used,
                            const uint8_t i, const uint8_t j) const;
    bool isInBounds(const uint8_t i, const uint8_t j) const;
    bool isValid(const std::vector<std::vector<Cell>>& b) const;
    void getAllNbhs(const uint8_t i1, const uint8_t j1, const uint8_t i2, const uint8_t j2, std::vector<std::pair<uint8_t, uint8_t>>& nbhs) const;
    bool isCellBlocked(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const;
    bool invalidEmptyCells(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const;  // 2 cells
    void getSurroundingCells(const std::vector<std::pair<int, int>>& polyomino, std::set<std::pair<int, int>>& surr) const;
    bool oneOption(uint8_t& n, const uint8_t i, const uint8_t j) const;
    bool processCodeCrossSection(const std::string& code, const uint8_t i_idx, const uint8_t j_idx, std::set<std::pair<uint8_t,uint8_t>>& s) const;
public:
    void crossSection(std::vector<std::vector<bool>>& done, const std::vector<std::vector<std::string>>& codes, const uint8_t i, const uint8_t j);
public:
    std::vector<std::vector<Cell>> board;
    bool validFromFile = true;

    Fillomino(const std::string& file) {
        getBoardFromFile(file);
    }
    Fillomino(const Fillomino& other) : rows(other.rows), cols(other.cols), board(other.board) {}
    Fillomino(const int r, const int c, const std::vector<std::vector<Cell>>& b) : rows(r), cols(c), board(b) {}

    void print() const;
    bool isValid() const;
    bool isSolved() const;
    bool processCode(const std::string& code, const uint8_t i, const uint8_t j,
                     std::vector<std::vector<Cell>>& b, const uint8_t number = 33) const;
    int getPartialSize(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const;
    bool isCellAClue(const uint8_t i, const uint8_t j) const;
    uint8_t getNum(const uint8_t i, const uint8_t j) const;
    void completeOneOption();
    void certainCells(const std::vector<std::vector<std::string>>& codes);
    bool areSizesValid() const;

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }
};

#endif  // POLYOMINOES_FILLOMINO_H
