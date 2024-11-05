#include "solver.h"

void Solver::getGeneratedPolyominoes(const std::string& filename) {
    polyominoes.resize(9);
    
    std::ifstream file(filename);
    int size;
    std::string text;

    while (file >> size && std::getline(file, text)) {
        if (!text.empty() && text[0] == ' ') {
            text = text.substr(1);
        }
        polyominoes[size].push_back(text);
    }
    file.close();
}

void Solver::tescik() const {
    for (const auto& code : polyominoes[3]) {
        if (fillomino.processCode(code, 0, 2))
            std::cout << code << ' ';
    }
}
