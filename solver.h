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

    void getGeneratedPolyominoes(const std::string& filename);
public:
    Solver(const std::string& file) : fillomino("boards/1.txt"){
        getGeneratedPolyominoes(file);
    }

    void tescik() const;
};

#endif  // POLYOMINOES_SOLVER_H
