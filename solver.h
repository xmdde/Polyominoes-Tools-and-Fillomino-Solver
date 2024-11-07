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
    bool next_step(uint8_t i, uint8_t j, const std::vector<std::vector<Cell>>& b) const;

public:
    Solver(const std::string& file) : fillomino("boards/5.txt") {
        getGeneratedPolyominoes(file);
        rows = fillomino.getRows();
        cols = fillomino.getCols();
    }

    void solve();
    void tescik() const;
};

#endif  // POLYOMINOES_SOLVER_H
