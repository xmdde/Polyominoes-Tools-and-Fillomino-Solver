#ifndef POLYOMINOES_FILLOMINO_H
#define POLYOMINOES_FILLOMINO_H

#include <cstdint>
#include <string>
#include <vector>

#include "cell.h"

class Fillomino {
    int rows;
    int cols;
    std::vector<std::vector<Cell>> board;
    // vec of partially finished polyominoes??

    void getBoardFromFile(const std::string& file);
    void fillInCertainCells();
    uint8_t getPartialSize(const uint8_t i, const uint8_t j) const;
    void getPartialSizeHelp(uint8_t& size, std::vector<std::vector<bool>>& used, const uint8_t i,
                            const uint8_t j) const;
    bool isInBounds(const uint8_t i, const uint8_t j) const;
public:
    Fillomino(const std::string& file) {
        getBoardFromFile(file);
    }
    // dodaj sprawdzanie poprawnosci zanim stworzymy nowy obiekt
    Fillomino(const Fillomino& other) : rows(other.rows), cols(other.cols), board(other.board) {}
    
    void print() const;
};

#endif  // POLYOMINOES_FILLOMINO_H