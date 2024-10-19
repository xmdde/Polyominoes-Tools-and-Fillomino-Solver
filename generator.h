#ifndef POLYOMINOES_GENERATOR_H
#define POLYOMINOES_GENERATOR_H

#include <iostream>
#include <vector>
#include <utility>
#include <cstdint>
#include <stack>

class Generator {
    const int N;
    const int cols;
    uint8_t depth = 1;

    void nextStep(const std::vector<std::vector<int8_t>>& board,
                  const std::vector<std::pair<uint8_t, uint8_t>>& available_cells,
                  const std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list,
                  const std::vector<bool>& old_used, const uint8_t added_i, const uint8_t added_j, const uint8_t old_counter,
                  const uint8_t old_max_used);
    void addNewBorderCells(std::vector<std::vector<int8_t>>& board, std::vector<std::pair<uint8_t, uint8_t>>& border_cells,
                           const uint8_t i, const uint8_t j, uint8_t& counter);
    void updateAvailableBorderCells(std::vector<std::pair<uint8_t, uint8_t>>& available_cells,
                                    const std::vector<std::vector<int8_t>>& board, const uint8_t max_used);
    void chooseCell(std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, const std::vector<bool>& used,
                    const uint8_t i, const uint8_t j, const std::vector<std::vector<int8_t>>& board);  // update max used
    bool isInBounds(const uint8_t i, const uint8_t j) const;

public:
    Generator(const int depth) : N(depth), cols(2*(depth-1)) {}
    void generate();
};

#endif  // POLYOMINOES_GENERATOR_H
