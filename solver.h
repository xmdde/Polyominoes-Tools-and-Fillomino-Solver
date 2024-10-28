#ifndef POLYOMINOES_SOLVER_H
#define POLYOMINOES_SOLVER_H

#include <vector>
#include <string>

class Solver {
    std::vector<std::vector<std::string>> polyominoes;

    void getGeneratedPolyominoes(const std::string& file);
public:
    Solver(const std::string& file) {}
};

#endif  // POLYOMINOES_SOLVER_H