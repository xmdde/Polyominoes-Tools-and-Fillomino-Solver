#ifndef POLYOMINOES_SOLVER_H
#define POLYOMINOES_SOLVER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "fillomino.h"

class Solver {
    std::vector<std::vector<std::string>> polyominoes;
    Fillomino fillomino;
    int rows;
    int cols;

    void getGeneratedPolyominoes(const std::string& filename);
    bool nextStep(uint8_t i, uint8_t j, const std::vector<std::vector<Cell>>& b) const;
    bool completeEmptyCells();
    bool nextStepEmpty();

public:
    Solver(const std::string& file, const std::string& board_file);
    void solve();

    void test() {
        std::vector<std::vector<bool>> checked(rows, std::vector<bool>(cols, false));
        fillomino.crossSection(checked, polyominoes, 3, 5);
    }
};

#endif  // POLYOMINOES_SOLVER_H
