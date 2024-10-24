#ifndef POLYOMINOES_FILLOMINO_H
#define POLYOMINOES_FILLOMINO_H

#include <cstdint>
#include <string>
#include <vector>

class Fillomino {
    int rows;
    int cols;
    std::vector<std::vector<uint8_t>> board;
    std::vector<std::vector<bool>> finished;
    // vec of partially finished polyominoes??

    void getBoardFromFile(const std::string& file);
    void fillInCertainCells();
    void getPartialSize(uint8_t& size, const uint8_t i, const uint8_t j, const uint8_t pre) const;
    bool isInBounds(const uint8_t i, const uint8_t j) const;
public:
    Fillomino(const std::string& file) {}
    Fillomino(const Fillomino& other) : rows(other.rows), cols(other.cols), board(other.board), finished(other.finished) {}
};

#endif  // POLYOMINOES_FILLOMINO_H