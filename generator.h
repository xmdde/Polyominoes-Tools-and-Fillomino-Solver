#ifndef POLYOMINOES_GENERATOR_H
#define POLYOMINOES_GENERATOR_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

class Generator {
    const int N;
    const int cols;
    uint8_t depth = 1;
    std::ofstream file;

    void nextStep(const std::vector<std::vector<int8_t>>& board,
                  const std::vector<std::pair<uint8_t, uint8_t>>& available_cells,
                  const std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list,
                  const std::vector<bool>& old_used, const uint8_t added_i, const uint8_t added_j, const uint8_t old_counter,
                  const uint8_t old_max_used);
    void addNewBorderCells(std::vector<std::vector<int8_t>>& board, std::vector<std::pair<uint8_t, uint8_t>>& border_cells,
                           const uint8_t i, const uint8_t j, uint8_t& counter) const;
    void updateAvailableBorderCells(std::vector<std::pair<uint8_t, uint8_t>>& available_cells,
                                    const std::vector<std::vector<int8_t>>& board, const uint8_t max_used) const;
    void chooseCell(std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, const std::vector<bool>& used,
                    const uint8_t i, const uint8_t j, const std::vector<std::vector<int8_t>>& board);
    bool isInBounds(const int8_t i, const int8_t j) const;
    void dfs(std::string& polyomino, std::vector<bool>& used, size_t& visited, 
             const std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, const uint8_t curr) const;
    void generatePolyominoCodes(std::string& polyomino, const std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list);
    void generateAllOptions(const std::string& code);
public:
    Generator(const int d, const std::string& filename) : N(d), cols(2*(d-1)), file(filename, std::ios::app) {
        if (!file.is_open()) std::cerr << "Failed to open result file.\n";
    }
    void generate();
};

#endif  // POLYOMINOES_GENERATOR_H
