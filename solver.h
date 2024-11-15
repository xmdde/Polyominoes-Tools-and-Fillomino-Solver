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
    bool completeEmptyCells(); // ? void
    bool nextStepEmpty();

public:
    Solver(const std::string& file, const std::string& board_file);
    void solve();
    void test();
};

#endif  // POLYOMINOES_SOLVER_H
